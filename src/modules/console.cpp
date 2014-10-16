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
	std::string messageType = message.get("type", "").asString();

	if (messageType.compare("command") == 0) {
		std::string command = message.get("command", "").asString();

		if (!command.empty()) {
			commandHistory[connection].push_back(command);
			engine->ClientCmd_Unrestricted(command.c_str());
		}
	}
	else if (messageType.compare("autocomplete") == 0) {
		std::string partial = message.get("partial", "").asString();

		std::vector<std::string> autoCompleteResults = GetAutoComplete(connection, partial);

		Json::Value message;
		message["type"] = "autocompleteresults";
		message["partial"] = partial;
		for (auto iterator = autoCompleteResults.begin(); iterator != autoCompleteResults.end(); ++iterator) {
			message["results"].append(*iterator);
		}

		std::thread sendMessage(std::bind(&WebSockets::SendPrivateMessage, g_WebSockets, connection, message));
		sendMessage.detach();
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

std::vector<std::string> Console::GetAutoComplete(websocketpp::connection_hdl connection, std::string partial) {
	std::vector<std::string> results;

	if (partial.length() == 0) {
		for (auto iterator = commandHistory[connection].rbegin(); iterator != commandHistory[connection].rend(); ++iterator) {
			results.push_back(*iterator);
		}
	}
	else if (partial.find(" ") == std::string::npos) {
		for (ConCommandBase const *cmd = (ConCommandBase const *)cvar->GetCommands(); cmd; cmd = cmd->GetNext()) {
			if (cmd->IsFlagSet(FCVAR_DEVELOPMENTONLY) || cmd->IsFlagSet(FCVAR_HIDDEN)) {
				continue;
			}

			if (strnicmp(partial.c_str(), cmd->GetName(), partial.length()) == 0) {
				results.push_back(cmd->GetName());
			}
		}

		std::sort(results.begin(), results.end());
	}
	else {
		std::stringstream ss(partial);
		std::string commandName;
		std::getline(ss, commandName, ' ');

		ConCommand *command = g_pCVar->FindCommand(commandName.c_str());
		if (command && command->CanAutoComplete()) {
			CUtlVector<CUtlString> matches;

			int count = command->AutoCompleteSuggest(partial.c_str(), matches);

			for (int i = 0; i < count; i++) {
				results.push_back(matches[i].String());
			}
		}
	}

	return results;
}