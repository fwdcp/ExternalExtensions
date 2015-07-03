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

#include <functional>
#include <thread>

#include <boost/asio.hpp>

#include "tier0/valve_minmax_on.h"
#include "cbase.h"
#include "c_baseentity.h"
#include "cdll_int.h"
#include "dbg.h"
#include "icliententity.h"
#include "inetchannelinfo.h"
#include "steam/steam_api.h"
#include "tier0/valve_minmax_off.h"

#include "../common.h"
#include "../gamedata.h"
#include "../gamethread.h"
#include "../ifaces.h"
#include "../player.h"
#include "../tfdefs.h"

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
		g_GameThreadHelper->AddCall(std::bind(&GameInfo::GetGameInfo, this, connection));
	}
}

void GameInfo::GetGameInfo(websocketpp::connection_hdl connection) {
	Json::Value message;
	message["type"] = "gameinfo";

	CSteamID steamID = Interfaces::pSteamAPIContext->SteamUser()->GetSteamID();

	message["client"]["name"] = Interfaces::pSteamAPIContext->SteamFriends()->GetPersonaName();
	message["client"]["steam"] = Json::valueToString(steamID.ConvertToUint64());

	message["ingame"] = Interfaces::pEngineClient->IsInGame();

	if (Interfaces::pEngineClient->IsInGame()) {
		if (Interfaces::pEngineClient->IsPlayingDemo()) {
			message["context"]["type"] = "demo";
			message["context"]["tick"] = Interfaces::pEngineClient->GetDemoPlaybackTick();
			message["context"]["paused"] = Interfaces::pEngineClient->IsPaused();
		}
		else {
			if (Interfaces::pEngineClient->IsHLTV()) {
				message["context"]["type"] = "sourcetv";
			}
			else {
				message["context"]["type"] = "game";
			}

			ConVar *password = g_pCVar->FindVar("password");

			if (password) {
				message["context"]["password"] = password->GetString();
			}

			INetChannelInfo *channel = Interfaces::pEngineClient->GetNetChannelInfo();

			if (channel) {
				message["context"]["address"] = channel->GetAddress();
			}
		}

		message["map"] = Interfaces::pEngineClient->GetLevelName();
		message["players"] = Json::Value(Json::arrayValue);

		for (Player player : Player::Iterable()) {
			if (player) {
				Json::Value playerJson;
				playerJson["index"] = player->entindex();

				if (Player::localPlayerCheckAvailable) {
					playerJson["local"] = player.IsLocalPlayer();
				}

				if (Player::nameRetrievalAvailable) {
					playerJson["name"] = player.GetName();
				}

				if (Player::steamIDRetrievalAvailable) {
					playerJson["steam"] = Json::valueToString(player.GetSteamID().ConvertToUint64());
				}

				playerJson["obsmode"] = player.GetObserverMode();
				if (player.GetObserverTarget()) {
					C_BaseEntity *targetEntity = player.GetObserverTarget();
					playerJson["obstarget"] = targetEntity->entindex();
				}

				TFTeam playerTeam = player.GetTeam();

				if (playerTeam == TFTeam_Spectator) {
					playerJson["team"] = "spectator";
				}
				else if (playerTeam == TFTeam_Red) {
					playerJson["team"] = "red";
				}
				else if (playerTeam == TFTeam_Blue) {
					playerJson["team"] = "blue";
				}

				playerJson["alive"] = player.IsAlive();

				playerJson["position"]["x"] = player.GetPosition().x;
				playerJson["position"]["y"] = player.GetPosition().y;
				playerJson["position"]["z"] = player.GetPosition().z;

				playerJson["health"] = player.GetHealth();
				playerJson["maxhealth"] = player.GetMaxHealth();

				if (Player::classRetrievalAvailable) {
					TFClassType playerClass = player.GetClass();

					if (playerClass == TFClass_Scout) {
						playerJson["class"] = "scout";
					}
					else if (playerClass == TFClass_Soldier) {
						playerJson["class"] = "soldier";
					}
					else if (playerClass == TFClass_Pyro) {
						playerJson["class"] = "pyro";
					}
					else if (playerClass == TFClass_DemoMan) {
						playerJson["class"] = "demoman";
					}
					else if (playerClass == TFClass_Heavy) {
						playerJson["class"] = "heavyweapons";
					}
					else if (playerClass == TFClass_Engineer) {
						playerJson["class"] = "engineer";
					}
					else if (playerClass == TFClass_Medic) {
						playerJson["class"] = "medic";
					}
					else if (playerClass == TFClass_Sniper) {
						playerJson["class"] = "sniper";
					}
					else if (playerClass == TFClass_Spy) {
						playerJson["class"] = "spy";
					}
				}

				if (Player::conditionsRetrievalAvailable) {
					for (int i = 0; i < 128; i++) {
						playerJson["condition"][i] = player.CheckCondition((TFCond)i);
					}
				}

				message["players"].append(playerJson);
			}
		}
	}

	std::thread sendMessage(std::bind(&WebSockets::SendPrivateMessage, g_WebSockets, connection, message));
	sendMessage.detach();
}
