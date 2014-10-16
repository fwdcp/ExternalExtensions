/*
 *  console.h
 *  ExternalExtensions project
 *
 *  Copyright (c) 2014 thesupremecommander
 *  MIT License
 *  http://opensource.org/licenses/MIT
 *
 */

#pragma once

#include <sstream>

#include "Color.h"
#include "icvar.h"

#include "../externalextensions.h"

class Console : public IConsoleDisplayFunc {
public:
	Console();
	void ReceiveMessage(websocketpp::connection_hdl connection, Json::Value message);
	virtual void ColorPrint(const Color& clr, const char *pMessage);
	virtual void Print(const char *pMessage);
	virtual void DPrint(const char *pMessage);
private:
	std::vector<std::string> GetAutoComplete(websocketpp::connection_hdl connection, std::string partial);
	std::map<websocketpp::connection_hdl, std::vector<std::string>, std::owner_less<websocketpp::connection_hdl>> commandHistory;
};