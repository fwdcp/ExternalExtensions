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

#include <websocketpp/config/asio_no_tls.hpp>

#include "json/json.h"

#include "Color.h"
#include "icvar.h"

#include "../modules.h"

class Console : public Module, public IConsoleDisplayFunc {
public:
	Console(std::string name);

	static bool CheckDependencies(std::string name);

	void ReceiveMessage(websocketpp::connection_hdl connection, Json::Value message);
	virtual void ColorPrint(const Color& clr, const char *pMessage);
	virtual void Print(const char *pMessage);
	virtual void DPrint(const char *pMessage);
private:
	std::vector<std::string> GetAutoComplete(websocketpp::connection_hdl connection, std::string partial);
	void OnConVarChange(IConVar *var, const char *pOldValue, float flOldValue);
	std::map<websocketpp::connection_hdl, std::vector<std::string>, std::owner_less<websocketpp::connection_hdl>> commandHistory;
};