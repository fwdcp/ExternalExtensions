/**
 *  ifaces.cpp
 *  ExternalExtensions project
 *
 *  Copyright (c) 2015 thesupremecommander
 *  MIT License
 *  http://opensource.org/licenses/MIT
 *
 */

#include "ifaces.h"

#include "tier0/valve_minmax_on.h"
#include "cdll_int.h"
#include "engine/ivmodelinfo.h"
#include "entitylist_base.h"
#include "filesystem_init.h"
#include "icliententitylist.h"
#include "igameevents.h"
#include "ivrenderview.h"
#include "steam/steam_api.h"
#include "tier3/tier3.h"
#include "toolframework/ienginetool.h"
#include "vgui_controls/Controls.h"
#include "tier0/valve_minmax_off.h"

#include "exceptions.h"
#include "platform.h"

IBaseClientDLL *Interfaces::pClientDLL = nullptr;
IClientEntityList *Interfaces::pClientEntityList = nullptr;
IVEngineClient *Interfaces::pEngineClient = nullptr;
IEngineTool *Interfaces::pEngineTool = nullptr;
IFileSystem *Interfaces::pFileSystem = nullptr;
IGameEventManager2 *Interfaces::pGameEventManager = nullptr;
IVModelInfoClient *Interfaces::pModelInfoClient = nullptr;
IVRenderView *Interfaces::pRenderView = nullptr;
CSteamAPIContext *Interfaces::pSteamAPIContext = nullptr;

bool Interfaces::steamLibrariesAvailable = false;
bool Interfaces::vguiLibrariesAvailable = false;

CBaseEntityList *g_pEntityList;

void Interfaces::Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory) {
	ConnectTier1Libraries(&interfaceFactory, 1);
	ConnectTier2Libraries(&interfaceFactory, 1);
	ConnectTier3Libraries(&interfaceFactory, 1);

	vguiLibrariesAvailable = vgui::VGui_InitInterfacesList("ExternalExtensions", &interfaceFactory, 1);

	pEngineClient = (IVEngineClient *)interfaceFactory(VENGINE_CLIENT_INTERFACE_VERSION, nullptr);
	pEngineTool = (IEngineTool *)interfaceFactory(VENGINETOOL_INTERFACE_VERSION, nullptr);
	pGameEventManager = (IGameEventManager2 *)interfaceFactory(INTERFACEVERSION_GAMEEVENTSMANAGER2, nullptr);
	pModelInfoClient = (IVModelInfoClient *)interfaceFactory(VMODELINFO_CLIENT_INTERFACE_VERSION, nullptr);
	pRenderView = (IVRenderView *)interfaceFactory(VENGINE_RENDERVIEW_INTERFACE_VERSION, nullptr);

	CreateInterfaceFn gameClientFactory;
	pEngineTool->GetClientFactory(gameClientFactory);

	pClientDLL = (IBaseClientDLL*)gameClientFactory(CLIENT_DLL_INTERFACE_VERSION, nullptr);
	pClientEntityList = (IClientEntityList*)gameClientFactory(VCLIENTENTITYLIST_INTERFACE_VERSION, nullptr);

	pSteamAPIContext = new CSteamAPIContext();
	steamLibrariesAvailable = SteamAPI_InitSafe() && pSteamAPIContext->Init();

	g_pEntityList = dynamic_cast<CBaseEntityList *>(Interfaces::pClientEntityList);

	char dll[MAX_PATH];
	bool steam;
	if (FileSystem_GetFileSystemDLLName(dll, sizeof(dll), steam) == FS_OK) {
		CFSLoadModuleInfo fsLoadModuleInfo;
		fsLoadModuleInfo.m_bSteam = steam;
		fsLoadModuleInfo.m_pFileSystemDLLName = dll;
		fsLoadModuleInfo.m_ConnectFactory = interfaceFactory;

		if (FileSystem_LoadFileSystemModule(fsLoadModuleInfo) == FS_OK) {
			CFSMountContentInfo fsMountContentInfo;
			fsMountContentInfo.m_bToolsMode = fsLoadModuleInfo.m_bToolsMode;
			fsMountContentInfo.m_pDirectoryName = fsLoadModuleInfo.m_GameInfoPath;
			fsMountContentInfo.m_pFileSystem = fsLoadModuleInfo.m_pFileSystem;

			if (FileSystem_MountContent(fsMountContentInfo) == FS_OK) {
				CFSSearchPathsInit fsSearchPathsInit;
				fsSearchPathsInit.m_pDirectoryName = fsLoadModuleInfo.m_GameInfoPath;
				fsSearchPathsInit.m_pFileSystem = fsLoadModuleInfo.m_pFileSystem;

				if (FileSystem_LoadSearchPaths(fsSearchPathsInit) == FS_OK) {
					Interfaces::pFileSystem = fsLoadModuleInfo.m_pFileSystem;
				}
			}
		}
	}
}

void Interfaces::Unload() {
	DisconnectTier3Libraries();
	DisconnectTier2Libraries();
	DisconnectTier1Libraries();

	pSteamAPIContext->Clear();

	pClientDLL = nullptr;
	pClientEntityList = nullptr;
	pEngineClient = nullptr;
	pEngineTool = nullptr;
	pFileSystem = nullptr;
	pGameEventManager = nullptr;
	pRenderView = nullptr;
}
