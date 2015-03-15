/*
 *  externalextensions.cpp
 *  ExternalExtensions project
 *
 *  Copyright (c) 2014 thesupremecommander
 *  MIT License
 *  http://opensource.org/licenses/MIT
 *
 */

#include "externalextensions.h"

WebSockets *g_WebSockets = nullptr;

Console *g_Console = nullptr;

IVEngineClient *engine = NULL;

//
// The plugin is a static singleton that is exported as an interface
//
ExternalExtensionsPlugin g_ExternalExtensionsPlugin;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(ExternalExtensionsPlugin, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, g_ExternalExtensionsPlugin );

ExternalExtensionsPlugin::ExternalExtensionsPlugin() {}
ExternalExtensionsPlugin::~ExternalExtensionsPlugin() {}

bool ExternalExtensionsPlugin::Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory) {
	g_WebSockets = new WebSockets();

	ConnectTier1Libraries(&interfaceFactory, 1);
	ConnectTier2Libraries(&interfaceFactory, 1);
	ConnectTier3Libraries(&interfaceFactory, 1);

	engine = (IVEngineClient *)interfaceFactory(VENGINE_CLIENT_INTERFACE_VERSION, NULL);

	ConVar_Register();

	g_Console = new Console();

	return true;
}

void ExternalExtensionsPlugin::Unload(void) {}
void ExternalExtensionsPlugin::Pause(void) {}
void ExternalExtensionsPlugin::UnPause(void) {}
const char *ExternalExtensionsPlugin::GetPluginDescription(void) { return PLUGIN_DESC; }
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