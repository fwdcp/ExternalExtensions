/**
 *  gamedata.h
 *  ExternalExtensions project
 *
 *  Copyright (c) 2015 thesupremecommander
 *  MIT License
 *  http://opensource.org/licenses/MIT
 *
 */

#pragma once

#include <string>

#ifdef _WIN32
#include <Windows.h>
#include <Psapi.h>
#undef GetClassName
#undef SendMessage
#endif

// DLL loading info
#if defined _WIN32
#define GetFuncAddress(pAddress, szFunction) ::GetProcAddress((HMODULE)pAddress, szFunction)
#define GetHandleOfModule(szModuleName) GetModuleHandle((std::string(szModuleName) + ".dll").c_str())
#elif defined __linux__
#define GetFuncAddress(pAddress, szFunction) dlsym(pAddress, szFunction)
#define GetHandleOfModule(szModuleName) dlopen((std::string(szModuleName) + ".so").c_str(), RTLD_NOLOAD)
#endif

// client DLL info
#if defined _WIN32
#define CLIENT_MODULE_FILE "tf/bin/client.dll"
#elif defined __APPLE__
#define CLIENT_MODULE_FILE "tf/bin/client.dylib"
#elif defined __linux__
#define CLIENT_MODULE_FILE "tf/bin/client.so"
#endif