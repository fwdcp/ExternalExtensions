/**
 *  platform.h
 *  ExternalExtensions project
 *
 *  Copyright (c) 2015 thesupremecommander
 *  MIT License
 *  http://opensource.org/licenses/MIT
 *
 */

#pragma once

// enable C++11 on Windows
#if defined _WIN32
#define _WEBSOCKETPP_CPP11_THREAD_
#define _WEBSOCKETPP_CPP11_FUNCTIONAL_
#define _WEBSOCKETPP_CPP11_SYSTEM_ERROR_
#define _WEBSOCKETPP_CPP11_RANDOM_DEVICE_
#define _WEBSOCKETPP_CPP11_MEMORY_
#define _WEBSOCKETPP_CPP11_CHRONO_
#endif

// client DLL info
#if defined _WIN32
#define CLIENT_MODULE_FILE "tf/bin/client.dll"
#elif defined __APPLE__
#define CLIENT_MODULE_FILE "tf/bin/client.dylib"
#elif defined __linux__
#define CLIENT_MODULE_FILE "tf/bin/client.so"
#endif
