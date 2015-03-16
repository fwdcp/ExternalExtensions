/*
 *  gameinfo.cpp
 *  ExternalExtensions project
 *
 *  Copyright (c) 2014 thesupremecommander
 *  MIT License
 *  http://opensource.org/licenses/MIT
 *
 */

#include "gameinfo.h"

#include <thread>

#include <boost/asio.hpp>

#include "cbase.h"
#include "c_baseentity.h"
#include "cdll_int.h"
#include "dbg.h"
#include "icliententity.h"
#include "inetchannelinfo.h"
#include "steam/steam_api.h"

#include "../common.h"
#include "../gamedata.h"
#include "../ifaces.h"
#include "../player.h"
#include "../tfdefs.h"
#include "../websockets.h"

GameInfo::GameInfo(std::string name) : Module(name) {
	g_WebSockets->RegisterMessageHook(std::bind(&GameInfo::ReceiveMessage, this, std::placeholders::_1, std::placeholders::_2));
}

bool GameInfo::CheckDependencies(std::string name) {
	bool ready = true;

	if (!Interfaces::pEngineClient) {
		PRINT_TAG();
		Warning("Required interface IVEngineClient for module %s not available!\n", name.c_str());

		ready = false;
	}

	if (!Player::CheckDependencies()) {
		PRINT_TAG();
		Warning("Player helper class for module %s not available!\n", name.c_str());

		ready = false;
	}

	if (!Interfaces::steamLibrariesAvailable) {
		PRINT_TAG();
		Warning("Required Steam libraries for module %s not available!\n", name.c_str());

		ready = false;
	}

	return ready;
}

void GameInfo::ReceiveMessage(websocketpp::connection_hdl connection, Json::Value message) {
	std::string messageType = message.get("type", "").asString();

	if (messageType.compare("gameinforequest") == 0) {
		Json::Value message;
		message["type"] = "gameinfo";
		message["players"] = Json::Value(Json::arrayValue);
		
		CSteamID steamID = Interfaces::pSteamAPIContext->SteamUser()->GetSteamID();

		message["client"]["name"] = Interfaces::pSteamAPIContext->SteamFriends()->GetPersonaName();
		message["client"]["steam"] = Json::valueToString(steamID.ConvertToUint64());

		if (Interfaces::pEngineClient->IsPlayingDemo()) {
			message["game"]["type"] = "demo";
			message["game"]["tick"] = Interfaces::pEngineClient->GetDemoPlaybackTick();
			message["game"]["paused"] = Interfaces::pEngineClient->IsPaused();
		}
		else if (Interfaces::pEngineClient->IsConnected()) {
			if (Interfaces::pEngineClient->IsHLTV()) {
				message["game"]["type"] = "sourcetv";
			}
			else {
				message["game"]["type"] = "game";
			}

			ConVar *password = g_pCVar->FindVar("password");

			if (password) {
				message["game"]["password"] = password->GetString();
			}

			INetChannelInfo *channel = Interfaces::pEngineClient->GetNetChannelInfo();

			if (channel) {
				message["game"]["address"] = channel->GetAddress();
			}
		}

		message["map"] = Interfaces::pEngineClient->GetLevelName();

		for (Player player : Player::Iterable()) {
			if (player) {
				Json::Value playerJson;
				playerJson["index"] = player->entindex();

				if (Player::classRetrievalAvailable) {
					playerJson["class"] = player.GetClass();
				}

				if (Player::conditionsRetrievalAvailable) {
					for (int i = 0; i < 128; i++) {
						playerJson["condition"][i] = player.CheckCondition((TFCond)i);
					}
				}

				if (Player::localPlayerCheckAvailable) {
					playerJson["local"] = player.IsLocalPlayer();
				}

				if (Player::nameRetrievalAvailable) {
					playerJson["name"] = player.GetName();
				}

				if (Player::steamIDRetrievalAvailable) {
					playerJson["steam"] = Json::valueToString(player.GetSteamID().ConvertToUint64());
				}

				playerJson["alive"] = player.IsAlive();

				playerJson["health"] = player.GetHealth();
				playerJson["maxhealth"] = player.GetMaxHealth();

				playerJson["obsmode"] = player.GetObserverMode();
				if (player.GetObserverTarget()) {
					C_BaseEntity *targetEntity = player.GetObserverTarget();
					playerJson["obstarget"] = targetEntity->entindex();
				}

				playerJson["position"]["x"] = player.GetPosition().x;
				playerJson["position"]["y"] = player.GetPosition().y;
				playerJson["position"]["z"] = player.GetPosition().z;

				playerJson["team"] = player.GetTeam();

				message["players"].append(playerJson);
			}
		}

		std::thread sendMessage(std::bind(&WebSockets::SendPrivateMessage, g_WebSockets, connection, message));
		sendMessage.detach();
	}
}