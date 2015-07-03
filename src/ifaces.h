/**
 *  ifaces.h
 *  ExternalExtensions project
 *
 *  Copyright (c) 2015 thesupremecommander
 *  MIT License
 *  http://opensource.org/licenses/MIT
 *
 */

#pragma once

#include "minmax.h"
#include "interface.h"
#undef MINMAX_H
#include "tier0/valve_minmax_off.h"

class C_HLTVCamera;
class C_TeamplayRoundBasedRules;
class CGlobalVarsBase;
class CSteamAPIContext;
class IBaseClientDLL;
class IClientEntityList;
class IClientMode;
class IFileSystem;
class IGameEventManager2;
class IGameResources;
class IVEngineClient;
class IVModelInfoClient;
class IVRenderView;

class Interfaces {
	public:
		static void Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory);
		static void Unload();

		static IBaseClientDLL *pClientDLL;
		static IClientEntityList *pClientEntityList;
		static IVEngineClient *pEngineClient;
		static IGameEventManager2 *pGameEventManager;
		static IVModelInfoClient *pModelInfoClient;
		static IVRenderView *pRenderView;
		static CSteamAPIContext *pSteamAPIContext;

		static bool steamLibrariesAvailable;
		static bool vguiLibrariesAvailable;
	private:
		static CDllDemandLoader *pClientModule;
};
