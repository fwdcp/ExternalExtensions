/**
 *  common.h
 *  ExternalExtensions project
 *
 *  Copyright (c) 2015 thesupremecommander
 *  MIT License
 *  http://opensource.org/licenses/MIT
 *
 */

#pragma once

#include <sstream>
#include <string>
#include <vector>

#include "Color.h"
#include "dbg.h"

inline std::string ConvertTreeToString(std::vector<std::string> tree) {
	std::stringstream ss;
	std::string string;

	for (auto iterator = tree.begin(); iterator != tree.end(); ++iterator) {
		ss << ">";
		ss << *iterator;
	}

	ss >> string;

	return string;
}

#define PLUGIN_VERSION "0.1.1"
#define PRINT_TAG() ConColorMsg(Color(0, 153, 153, 255), "[ExternalExtensions] ")