/*
 *  console.cpp
 *  ExternalExtensions project
 *
 *  Copyright (c) 2015 thesupremecommander
 *  MIT License
 *  http://opensource.org/licenses/MIT
 *
 */

#include "console.h"

#include <thread>

#include "cdll_int.h"

#include "../common.h"
#include "../ifaces.h"
#include "../websockets.h"

Console::Console(std::string name) : Module(name) {
	g_pCVar->InstallGlobalChangeCallback([](IConVar *var, const char *pOldValue, float flOldValue) { g_ModuleManager->GetModule<Console>("Console")->OnConVarChange(var, pOldValue, flOldValue); });
	g_pCVar->InstallConsoleDisplayFunc(this);

	g_WebSockets->RegisterMessageHook(std::bind(&Console::ReceiveMessage, this, std::placeholders::_1, std::placeholders::_2));
}

bool Console::CheckDependencies(std::string name) {
	bool ready = true;

	if (!Interfaces::pEngineClient) {
		PRINT_TAG();
		Warning("Required interface IVEngineClient for module %s not available!\n", name.c_str());

		ready = false;
	}

	return ready;
}

void Console::ReceiveMessage(websocketpp::connection_hdl connection, Json::Value message) {
	std::string messageType = message.get("type", "").asString();

	if (messageType.compare("command") == 0) {
		std::string command = message.get("command", "").asString();

		if (!command.empty()) {
			commandHistory[connection].push_back(command);
			Interfaces::pEngineClient->ClientCmd_Unrestricted(command.c_str());
		}
	}
	else if (messageType.compare("convarchange") == 0) {
		const char *name = message.get("name", "").asCString();

		ConVar *convar = g_pCVar->FindVar(name);

		if (convar) {
			convar->SetValue(message.get("value", "").asCString());
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

void Console::OnConVarChange(IConVar *var, const char *pOldValue, float flOldValue) {
	Json::Value message;
	message["type"] = "convarchanged";
	message["name"] = var->GetName();
	message["oldvalue"] = pOldValue;
	message["newvalue"] = g_pCVar->FindVar(var->GetName())->GetString();

	std::thread sendMessage(std::bind(&WebSockets::SendGlobalMessage, g_WebSockets, message));
	sendMessage.detach();
}