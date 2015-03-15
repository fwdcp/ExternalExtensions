/*
 *  modules.cpp
 *  ExternalExtensions project
 *
 *  Copyright (c) 2015 thesupremecommander
 *  MIT License
 *  http://opensource.org/licenses/MIT
 *
 */

#pragma once

#include "modules.h"

#include "common.h"

void ModuleManager::UnloadAllModules() {
	for (auto iterator = modules.begin(); iterator != modules.end(); ++iterator) {
		PRINT_TAG();
		ConColorMsg(Color(0, 255, 0, 255), "Module %s unloaded!\n", iterator->first.c_str());

		delete iterator->second;
	}

	modules.clear();
}