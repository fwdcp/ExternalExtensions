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
#include "vgui_controls/Controls.h"
#include "tier0/valve_minmax_off.h"

#include "exceptions.h"
#include "platform.h"

IBaseClientDLL *Interfaces::pClientDLL = nullptr;
IClientEntityList *Interfaces::pClientEntityList = nullptr;
CDllDemandLoader *Interfaces::pClientModule = nullptr;
IVEngineClient *Interfaces::pEngineClient = nullptr;
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

	vguiLibrariesAvailable = vgui::VGui_InitInterfacesList("externalextensions", &interfaceFactory, 1);

	pEngineClient = (IVEngineClient *)interfaceFactory(VENGINE_CLIENT_INTERFACE_VERSION, nullptr);
	pGameEventManager = (IGameEventManager2 *)interfaceFactory(INTERFACEVERSION_GAMEEVENTSMANAGER2, nullptr);
	pModelInfoClient = (IVModelInfoClient *)interfaceFactory(VMODELINFO_CLIENT_INTERFACE_VERSION, nullptr);
	pRenderView = (IVRenderView *)interfaceFactory(VENGINE_RENDERVIEW_INTERFACE_VERSION, nullptr);

	pClientModule = new CDllDemandLoader(CLIENT_MODULE_FILE);

	CreateInterfaceFn gameClientFactory = pClientModule->GetFactory();

	pClientDLL = (IBaseClientDLL*)gameClientFactory(CLIENT_DLL_INTERFACE_VERSION, nullptr);
	pClientEntityList = (IClientEntityList*)gameClientFactory(VCLIENTENTITYLIST_INTERFACE_VERSION, nullptr);

	pSteamAPIContext = new CSteamAPIContext();
	steamLibrariesAvailable = SteamAPI_InitSafe() && pSteamAPIContext->Init();

	g_pEntityList = (CBaseEntityList *) Interfaces::pClientEntityList;
}

void Interfaces::Unload() {
	DisconnectTier3Libraries();
	DisconnectTier2Libraries();
	DisconnectTier1Libraries();

	pSteamAPIContext->Clear();

	pClientModule->Unload();
	pClientModule = nullptr;

	pClientDLL = nullptr;
	pClientEntityList = nullptr;
	pEngineClient = nullptr;
	pGameEventManager = nullptr;
	pRenderView = nullptr;
}
