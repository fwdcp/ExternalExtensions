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

#include "tier0/valve_minmax_on.h"
#include "interface.h"
#include "tier0/valve_minmax_off.h"

class CSteamAPIContext;
class IBaseClientDLL;
class IClientEntityList;
class IEngineTool;
class IFileSystem;
class IGameEventManager2;
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
		static IEngineTool *pEngineTool;
		static IFileSystem *pFileSystem;
		static IGameEventManager2 *pGameEventManager;
		static IVModelInfoClient *pModelInfoClient;
		static IVRenderView *pRenderView;
		static CSteamAPIContext *pSteamAPIContext;

		static bool steamLibrariesAvailable;
		static bool vguiLibrariesAvailable;
};
