/*
 *  externalextensions.cpp
 *  ExternalExtensions project
 *
 *  Copyright (c) 2015 thesupremecommander
 *  MIT License
 *  http://opensource.org/licenses/MIT
 *
 */

#include "externalextensions.h"

#include "convar.h"

#include "common.h"
#include "ifaces.h"
#include "modules.h"
#include "websockets.h"

#include "modules/console.h"

ModuleManager *g_ModuleManager = nullptr;

WebSockets *g_WebSockets = nullptr;

// The plugin is a static singleton that is exported as an interface
ExternalExtensionsPlugin g_ExternalExtensionsPlugin;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(ExternalExtensionsPlugin, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, g_ExternalExtensionsPlugin);

ExternalExtensionsPlugin::ExternalExtensionsPlugin() {}
ExternalExtensionsPlugin::~ExternalExtensionsPlugin() {}

bool ExternalExtensionsPlugin::Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory) {
	PRINT_TAG();
	ConColorMsg(Color(0, 255, 255, 255), "version %s | a Forward Command Post project (http://fwdcp.net)\n", PLUGIN_VERSION);

	PRINT_TAG();
	ConColorMsg(Color(255, 255, 0, 255), "Loading plugin...\n");

	Interfaces::Load(interfaceFactory, gameServerFactory);

	g_WebSockets = new WebSockets();

	g_ModuleManager = new ModuleManager();

	g_ModuleManager->LoadModule<Console>("Console");

	ConVar_Register();

	PRINT_TAG();
	ConColorMsg(Color(0, 255, 0, 255), "Finished loading!\n");

	return true;
}

void ExternalExtensionsPlugin::Unload(void) {
	PRINT_TAG();
	ConColorMsg(Color(255, 255, 0, 255), "Unloading plugin...\n");

	g_ModuleManager->UnloadAllModules();

	ConVar_Unregister();
	Interfaces::Unload();

	PRINT_TAG();
	ConColorMsg(Color(0, 255, 0, 255), "Finished unloading!\n");
}

void ExternalExtensionsPlugin::Pause(void) {}
void ExternalExtensionsPlugin::UnPause(void) {}

const char *ExternalExtensionsPlugin::GetPluginDescription(void) {
	std::stringstream ss;
	std::string desc;

	ss << "ExternalExtensions " << PLUGIN_VERSION;
	ss >> desc;

	return desc.c_str();
}

void ExternalExtensionsPlugin::LevelInit(char const *pMapName) {}
void ExternalExtensionsPlugin::ServerActivate(edict_t *pEdictList, int edictCount, int clientMax) {}
void ExternalExtensionsPlugin::GameFrame(bool simulating) {}
void ExternalExtensionsPlugin::LevelShutdown(void) {}
void ExternalExtensionsPlugin::ClientActive(edict_t *pEntity) {}
void ExternalExtensionsPlugin::ClientDisconnect(edict_t *pEntity) {}
void ExternalExtensionsPlugin::ClientPutInServer(edict_t *pEntity, char const *playername) {}
void ExternalExtensionsPlugin::SetCommandClient(int index) {}
void ExternalExtensionsPlugin::ClientSettingsChanged(edict_t *pEdict) {}
PLUGIN_RESULT ExternalExtensionsPlugin::ClientConnect(bool *bAllowConnect, edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen) { return PLUGIN_CONTINUE; }
PLUGIN_RESULT ExternalExtensionsPlugin::ClientCommand(edict_t *pEntity, const CCommand &args) { return PLUGIN_CONTINUE; }
PLUGIN_RESULT ExternalExtensionsPlugin::NetworkIDValidated(const char *pszUserName, const char *pszNetworkID) { return PLUGIN_CONTINUE; }
void ExternalExtensionsPlugin::OnQueryCvarValueFinished(QueryCvarCookie_t iCookie, edict_t *pPlayerEntity, EQueryCvarValueStatus eStatus, const char *pCvarName, const char *pCvarValue) {}
void ExternalExtensionsPlugin::OnEdictAllocated(edict_t *edict) {}
void ExternalExtensionsPlugin::OnEdictFreed(const edict_t *edict) {}