/*
 *  gamethread.cpp
 *  ExternalExtensions project
 *
 *  Copyright (c) 2015 thesupremecommander
 *  MIT License
 *  http://opensource.org/licenses/MIT
 *
 */

#include "gamethread.h"

#include "minmax.h"
#include "tier3/tier3.h"
#include "vgui/IVGui.h"
#undef MINMAX_H
#include "tier0/valve_minmax_off.h"

#include "ifaces.h"

GameThreadHelper::GameThreadHelper() : Panel() {
	g_pVGui->AddTickSignal(GetVPanel());
}

void GameThreadHelper::AddCall(std::function<void()> call) {
	std::unique_lock<std::mutex> lock(callQueueLock);

	callQueue.push(call);
}

void GameThreadHelper::OnTick() {
	std::unique_lock<std::mutex> lock(callQueueLock);

	while (!callQueue.empty()) {
		std::function<void()> call = callQueue.front();

		call();

		callQueue.pop();
	}
}
