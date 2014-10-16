/*
 *  console.cpp
 *  ExternalExtensions project
 *
 *  Copyright (c) 2014 thesupremecommander
 *  MIT License
 *  http://opensource.org/licenses/MIT
 *
 */

#include "console.h"

Console::Console() {
	g_pCVar->InstallConsoleDisplayFunc(this);

	g_WebSockets->RegisterMessageHook(std::bind(&Console::ReceiveMessage, this, std::placeholders::_1, std::placeholders::_2));
}

void Console::ReceiveMessage(websocketpp::connection_hdl connection, Json::Value message) {
	if (message.get("type", "").asString().compare("command") == 0) {
		std::string command = message.get("command", "").asString();

		engine->ClientCmd_Unrestricted(command.c_str());
	}
}

void Console::ColorPrint(const Color& clr, const char *pMessage) {
	Json::Value message;
	message["type"] = "consoleprint";
	message["message"]["type"] = "color";
	message["message"]["color"]["r"] = clr.r();
	message["message"]["color"]["g"] = clr.g();
	message["message"]["color"]["b"] = clr.b();
	message["message"]["color"]["a"] = clr.a();
	message["message"]["text"] = pMessage;

	std::thread sendMessage(std::bind(&WebSockets::SendGlobalMessage, g_WebSockets, message));
	sendMessage.detach();
}

void Console::Print(const char *pMessage) {
	Json::Value message;
	message["type"] = "consoleprint";
	message["message"]["type"] = "regular";
	message["message"]["text"] = pMessage;

	std::thread sendMessage(std::bind(&WebSockets::SendGlobalMessage, g_WebSockets, message));
	sendMessage.detach();
}

void Console::DPrint(const char *pMessage) {
	Json::Value message;
	message["type"] = "consoleprint";
	message["message"]["type"] = "debug";
	message["message"]["text"] = pMessage;

	std::thread sendMessage(std::bind(&WebSockets::SendGlobalMessage, g_WebSockets, message));
	sendMessage.detach();
}