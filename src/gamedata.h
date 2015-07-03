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

// client DLL info
#if defined _WIN32
#define CLIENT_MODULE_FILE "tf/bin/client.dll"
#elif defined __APPLE__
#define CLIENT_MODULE_FILE "tf/bin/client.dylib"
#elif defined __linux__
#define CLIENT_MODULE_FILE "tf/bin/client.so"
#endif
