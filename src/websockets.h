/*
 *  websockets.h
 *  ExternalExtensions project
 *
 *  Copyright (c) 2015 thesupremecommander
 *  MIT License
 *  http://opensource.org/licenses/MIT
 *
 */

#pragma once

#include <set>

#define _WEBSOCKETPP_CPP11_THREAD_
#define _WEBSOCKETPP_CPP11_FUNCTIONAL_
#define _WEBSOCKETPP_CPP11_SYSTEM_ERROR_
#define _WEBSOCKETPP_CPP11_RANDOM_DEVICE_
#define _WEBSOCKETPP_CPP11_MEMORY_

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/common/thread.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include <json/json.h>

#include "convar.h"

class WebSockets {
public:
	WebSockets();

	void Start();
	void Stop();

	boost::uuids::uuid RegisterConnectHook(std::function<void(websocketpp::connection_hdl)> function);
	boost::uuids::uuid RegisterDisconnectHook(std::function<void(websocketpp::connection_hdl)> function);
	boost::uuids::uuid RegisterMessageHook(std::function<void(websocketpp::connection_hdl, Json::Value)> function);
	void UnregisterConnectHook(boost::uuids::uuid id);
	void UnregisterDisconnectHook(boost::uuids::uuid id);
	void UnregisterMessageHook(boost::uuids::uuid id);
	void SendPrivateMessage(websocketpp::connection_hdl recipient, Json::Value message);
	void SendGlobalMessage(Json::Value message);

private:
	typedef enum {
		ActionType_Connected,
		ActionType_Disconnected,
		ActionType_IncomingMessage,
		ActionType_OutgoingPrivateMessage,
		ActionType_OutgoingGlobalMessage
	} ActionType;

	typedef struct {
		ActionType type;
		websocketpp::connection_hdl associatedConnection;
		Json::Value message;
	} Action;

	void OnOpen(websocketpp::connection_hdl connection);
	void OnClose(websocketpp::connection_hdl connection);
	void OnMessage(websocketpp::connection_hdl connection, websocketpp::server<websocketpp::config::asio>::message_ptr message);
	void ProcessEvents();
	
	websocketpp::server<websocketpp::config::asio> server;
	websocketpp::lib::thread runner;
	websocketpp::lib::thread processor;
	std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> currentConnections;
	std::queue<Action> actions;
	boost::uuids::random_generator uuidGenerator;
	std::map<boost::uuids::uuid, std::function<void(websocketpp::connection_hdl)>> connectHooks;
	std::map<boost::uuids::uuid, std::function<void(websocketpp::connection_hdl)>> disconnectHooks;
	std::map<boost::uuids::uuid, std::function<void(websocketpp::connection_hdl, Json::Value)>> messageHooks;
	websocketpp::lib::mutex actionLock;
	websocketpp::lib::condition_variable actionNotification;
	websocketpp::lib::mutex connectionLock;

	ConVar *port;
	ConCommand *start;
	ConCommand *stop;
};

extern WebSockets *g_WebSockets;