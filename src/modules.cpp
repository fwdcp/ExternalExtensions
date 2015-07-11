/*
 *  modules.cpp
 *  ExternalExtensions project
 *
 *  Copyright (c) 2015 thesupremecommander
 *  MIT License
 *  http://opensource.org/licenses/MIT
 *
 */

#include "modules.h"

#include "common.h"

void ModuleManager::UnloadAllModules() {
	for (auto module : modules) {
		PRINT_TAG();
		ConColorMsg(Color(0, 255, 0, 255), "Module %s unloaded!\n", module.first.c_str());

		delete module.second;
	}

	modules.clear();
}
