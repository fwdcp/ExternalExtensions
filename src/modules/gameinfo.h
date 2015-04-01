/*
 *  gameinfo.h
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

#include "../modules.h"

class GameInfo : public Module {
public:
	GameInfo(std::string name);

	static bool CheckDependencies(std::string name);

	void ReceiveMessage(websocketpp::connection_hdl connection, Json::Value message);
private:
	void GetGameInfo(websocketpp::connection_hdl connection);
};