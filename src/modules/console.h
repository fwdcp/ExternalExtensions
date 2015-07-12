/*
 *  console.h
 *  ExternalExtensions project
 *
 *  Copyright (c) 2015 thesupremecommander
 *  MIT License
 *  http://opensource.org/licenses/MIT
 *
 */

#pragma once

#include "json/json.h"

#include "tier0/valve_minmax_on.h"
#include "Color.h"
#include "icvar.h"
#include "tier0/valve_minmax_off.h"

#include "../modules.h"
#include "../websockets.h"

class Console : public Module, public IConsoleDisplayFunc {
public:
	Console(std::string name);

	static bool CheckDependencies(std::string name);

	void ReceiveMessage(websocketpp::connection_hdl connection, Json::Value message);
	virtual void ColorPrint(const Color& clr, const char *pMessage);
	virtual void Print(const char *pMessage);
	virtual void DPrint(const char *pMessage);
private:
	void ChangeConVar(std::string name, std::string value);
	void ExecCommand(websocketpp::connection_hdl connection, std::string command);
	void GetAutoComplete(websocketpp::connection_hdl connection, std::string partial);
	void QueryConVar(websocketpp::connection_hdl connection, std::string name);

	std::map<websocketpp::connection_hdl, std::vector<std::string>, std::owner_less<websocketpp::connection_hdl>> commandHistory;

	void OnConVarChange(IConVar *var, const char *pOldValue, float flOldValue);
};
