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

#include <functional>
#include <thread>

#include "cdll_int.h"

#include "../common.h"
#include "../gamethread.h"
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
			g_GameThreadHelper->AddCall(std::bind(&Console::ExecCommand, this, connection, command));
		}
	}
	else if (messageType.compare("convarquery") == 0) {
		std::string name = message.get("name", "").asString();

		if (!name.empty()) {
			g_GameThreadHelper->AddCall(std::bind(&Console::QueryConVar, this, connection, name));
		}
	}
	else if (messageType.compare("convarchange") == 0) {
		std::string name = message.get("name", "").asString();
		std::string value = message.get("value", "").asString();

		if (!name.empty()) {
			g_GameThreadHelper->AddCall(std::bind(&Console::ChangeConVar, this, name, value));
		}
	}
	else if (messageType.compare("autocomplete") == 0) {
		std::string partial = message.get("partial", "").asString();

		g_GameThreadHelper->AddCall(std::bind(&Console::GetAutoComplete, this, connection, partial));
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

void Console::ChangeConVar(std::string name, std::string value) {
	ConVar *convar = g_pCVar->FindVar(name.c_str());

	if (convar) {
		convar->SetValue(value.c_str());
	}
}

void Console::ExecCommand(websocketpp::connection_hdl connection, std::string command) {
	commandHistory[connection].push_back(command);
	Interfaces::pEngineClient->ClientCmd_Unrestricted(command.c_str());
}

void Console::GetAutoComplete(websocketpp::connection_hdl connection, std::string partial) {
	Json::Value message;
	message["type"] = "autocompleteresults";
	message["partial"] = partial;

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

	for (std::string result : results) {
		message["results"].append(result);
	}

	std::thread sendMessage(std::bind(&WebSockets::SendPrivateMessage, g_WebSockets, connection, message));
	sendMessage.detach();
}

void Console::QueryConVar(websocketpp::connection_hdl connection, std::string name) {
	Json::Value message;
	message["type"] = "convarqueryresult";
	message["name"] = name;

	ConVar *convar = g_pCVar->FindVar(name.c_str());

	if (convar) {
		message["exists"] = true;
		message["help"] = convar->GetHelpText();
		message["value"] = convar->GetString();
	}
	else {
		message["exists"] = false;
	}

	std::thread sendMessage(std::bind(&WebSockets::SendPrivateMessage, g_WebSockets, connection, message));
	sendMessage.detach();
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