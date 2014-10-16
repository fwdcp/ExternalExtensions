/*
 *  externalextensions.h
 *  ExternalExtensions project
 *
 *  Copyright (c) 2014 thesupremecommander
 *  MIT License
 *  http://opensource.org/licenses/MIT
 *
 */

#pragma once

#include <thread>

#include "cdll_int.h"
#include "engine/iserverplugin.h"
#include "tier3/tier3.h"

#include "websockets.h"

#include "modules/console.h"

#define PLUGIN_DESC "ExternalExtensions v0.0.1"

class ExternalExtensionsPlugin: public IServerPluginCallbacks
{
public:
	ExternalExtensionsPlugin();
	~ExternalExtensionsPlugin();

	// IServerPluginCallbacks methods
	virtual bool			Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory);
	virtual void			Unload(void);
	virtual void			Pause(void);
	virtual void			UnPause(void);
	virtual const char		*GetPluginDescription(void);
	virtual void			LevelInit(char const *pMapName);
	virtual void			ServerActivate(edict_t *pEdictList, int edictCount, int clientMax);
	virtual void			GameFrame(bool simulating);
	virtual void			LevelShutdown(void);
	virtual void			ClientActive(edict_t *pEntity);
	virtual void			ClientDisconnect(edict_t *pEntity);
	virtual void			ClientPutInServer(edict_t *pEntity, char const *playername);
	virtual void			SetCommandClient(int index);
	virtual void			ClientSettingsChanged(edict_t *pEdict);
	virtual PLUGIN_RESULT	ClientConnect(bool *bAllowConnect, edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen);
	virtual PLUGIN_RESULT	ClientCommand(edict_t *pEntity, const CCommand &args);
	virtual PLUGIN_RESULT	NetworkIDValidated(const char *pszUserName, const char *pszNetworkID);
	virtual void			OnQueryCvarValueFinished(QueryCvarCookie_t iCookie, edict_t *pPlayerEntity, EQueryCvarValueStatus eStatus, const char *pCvarName, const char *pCvarValue);
	virtual void			OnEdictAllocated(edict_t *edict);
	virtual void			OnEdictFreed(const edict_t *edict);
};

extern IVEngineClient *engine;