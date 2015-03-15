/*
 *  websockets.cpp
 *  ExternalExtensions project
 *
 *  Copyright (c) 2014 thesupremecommander
 *  MIT License
 *  http://opensource.org/licenses/MIT
 *
 */

#include "websockets.h"

#include "dbg.h"

WebSockets::WebSockets() {
	port = new ConVar("externalextensions_port", "2006", FCVAR_NONE, "port to run the WebSockets server on");
	start = new ConCommand("externalextensions_start", []() { g_WebSockets->Start(); }, "start the WebSocket server (if not already running)", FCVAR_NONE);
	stop = new ConCommand("externalextensions_stop", []() { g_WebSockets->Stop(); }, "stop the WebSocket server (if already running)", FCVAR_NONE);

	try {
		server.init_asio();

		server.set_open_handler(websocketpp::lib::bind(&WebSockets::OnOpen, this, websocketpp::lib::placeholders::_1));
		server.set_close_handler(websocketpp::lib::bind(&WebSockets::OnClose, this, websocketpp::lib::placeholders::_1));
		server.set_message_handler(websocketpp::lib::bind(&WebSockets::OnMessage, this, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));

		processor = websocketpp::lib::thread(websocketpp::lib::bind(&WebSockets::ProcessEvents, this));
	}
	catch (std::exception &e) {
		Warning(e.what());
	}
}

void WebSockets::Start() {
	try {
		if (server.is_listening()) {
			Msg("WebSockets server is already running.\n");

			return;
		}

		server.listen(port->GetInt());
		server.start_accept();

		runner = websocketpp::lib::thread(websocketpp::lib::bind(&websocketpp::server<websocketpp::config::asio>::run, &server));
	}
	catch (std::exception &e) {
		Warning(e.what());
	}
}

void WebSockets::Stop() {
	try {
		if (!server.is_listening()) {
			Msg("WebSockets server is already not running.\n");

			return;
		}

		server.stop_listening();

		for (websocketpp::connection_hdl connection : currentConnections) {
			try {
				server.close(connection, websocketpp::close::status::going_away, "server shutting down");
			}
			catch (std::exception &e) {
				Warning(e.what());
			}
		}

		runner.join();

		server.stop();
		server.reset();
	}
	catch (std::invalid_argument &e) {
		Warning(e.what());
	}
}

boost::uuids::uuid WebSockets::RegisterConnectHook(std::function<void(websocketpp::connection_hdl)> function) {
	boost::uuids::uuid id = uuidGenerator();

	connectHooks[id] = function;

	return id;
}

boost::uuids::uuid WebSockets::RegisterDisconnectHook(std::function<void(websocketpp::connection_hdl)> function) {
	boost::uuids::uuid id = uuidGenerator();

	disconnectHooks[id] = function;

	return id;
}

boost::uuids::uuid WebSockets::RegisterMessageHook(std::function<void(websocketpp::connection_hdl, Json::Value)> function) {
	boost::uuids::uuid id = uuidGenerator();

	messageHooks[id] = function;

	return id;
}

void WebSockets::UnregisterConnectHook(boost::uuids::uuid id) {
	connectHooks.erase(id);
}

void WebSockets::UnregisterDisconnectHook(boost::uuids::uuid id) {
	disconnectHooks.erase(id);
}

void WebSockets::UnregisterMessageHook(boost::uuids::uuid id) {
	messageHooks.erase(id);
}

void WebSockets::SendPrivateMessage(websocketpp::connection_hdl recipient, Json::Value message) {
	websocketpp::lib::unique_lock<websocketpp::lib::mutex> lock(actionLock);

	Action privateMessage;
	privateMessage.type = ActionType_OutgoingPrivateMessage;
	privateMessage.associatedConnection = recipient;
	privateMessage.message = message;

	actions.push(privateMessage);

	lock.unlock();

	actionNotification.notify_one();
}

void WebSockets::SendGlobalMessage(Json::Value message) {
	websocketpp::lib::unique_lock<websocketpp::lib::mutex> lock(actionLock);

	Action globalMessage;
	globalMessage.type = ActionType_OutgoingGlobalMessage;
	globalMessage.message = message;

	actions.push(globalMessage);

	lock.unlock();

	actionNotification.notify_one();
}

void WebSockets::OnOpen(websocketpp::connection_hdl connection) {
	websocketpp::lib::unique_lock<websocketpp::lib::mutex> lock(actionLock);

	Action connect;
	connect.type = ActionType_Connected;
	connect.associatedConnection = connection;

	actions.push(connect);

	lock.unlock();

	actionNotification.notify_one();
}

void WebSockets::OnClose(websocketpp::connection_hdl connection) {
	websocketpp::lib::unique_lock<websocketpp::lib::mutex> lock(actionLock);

	Action disconnect;
	disconnect.type = ActionType_Disconnected;
	disconnect.associatedConnection = connection;

	actions.push(disconnect);

	lock.unlock();

	actionNotification.notify_one();
}

void WebSockets::OnMessage(websocketpp::connection_hdl connection, websocketpp::server<websocketpp::config::asio>::message_ptr message) {
	websocketpp::lib::unique_lock<websocketpp::lib::mutex> lock(actionLock);

	Action incomingMessage;
	incomingMessage.type = ActionType_IncomingMessage;
	incomingMessage.associatedConnection = connection;

	if (message->get_opcode() == websocketpp::frame::opcode::text) {
		Json::Reader reader;
		Json::Value json;

		if (reader.parse(message->get_payload(), json)) {
			incomingMessage.message = json;
		}
	}

	actions.push(incomingMessage);

	lock.unlock();

	actionNotification.notify_one();
}

void WebSockets::ProcessEvents() {
	while (true) {
		websocketpp::lib::unique_lock<websocketpp::lib::mutex> lock(actionLock);

		while (actions.empty()) {
			actionNotification.wait(lock);
		}

		Action action = actions.front();
		
		if (action.type == ActionType_Connected) {
			websocketpp::lib::unique_lock<websocketpp::lib::mutex> lock(connectionLock);

			currentConnections.insert(action.associatedConnection);

			for (auto iterator = connectHooks.begin(); iterator != connectHooks.end(); ++iterator) {
				iterator->second(action.associatedConnection);
			}
		}
		else if (action.type == ActionType_Disconnected) {
			websocketpp::lib::unique_lock<websocketpp::lib::mutex> lock(connectionLock);

			currentConnections.erase(action.associatedConnection);

			for (auto iterator = disconnectHooks.begin(); iterator != disconnectHooks.end(); ++iterator) {
				iterator->second(action.associatedConnection);
			}
		}
		else if (action.type == ActionType_IncomingMessage) {
			websocketpp::lib::unique_lock<websocketpp::lib::mutex> lock(connectionLock);

			for (auto iterator = messageHooks.begin(); iterator != messageHooks.end(); ++iterator) {
				iterator->second(action.associatedConnection, action.message);
			}
		}
		else if (action.type == ActionType_OutgoingPrivateMessage) {
			websocketpp::lib::unique_lock<websocketpp::lib::mutex> lock(connectionLock);

			Json::FastWriter writer;
			websocketpp::lib::error_code error;

			server.send(action.associatedConnection, writer.write(action.message), websocketpp::frame::opcode::text, error);

			if (error) {
				Warning(error.message().c_str());
			}
		}
		else if (action.type == ActionType_OutgoingGlobalMessage) {
			websocketpp::lib::unique_lock<websocketpp::lib::mutex> lock(connectionLock);

			Json::FastWriter writer;
			std::string payload = writer.write(action.message);

			for (auto iterator = currentConnections.begin(); iterator != currentConnections.end(); ++iterator) {
				websocketpp::lib::error_code error;

				server.send(*iterator, payload, websocketpp::frame::opcode::text, error);

				if (error) {
					Warning(error.message().c_str());
				}
				
				error.clear();
			}
		}

		actions.pop();
	}
}