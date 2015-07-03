/*
 *  gamethread.h
 *  ExternalExtensions project
 *
 *  Copyright (c) 2015 thesupremecommander
 *  MIT License
 *  http://opensource.org/licenses/MIT
 *
 */

#include <functional>
#include <mutex>
#include <queue>

#include "minmax.h"
#include "vgui_controls/Panel.h"
#undef null
#undef MINMAX_H
#include "tier0/valve_minmax_off.h"

class GameThreadHelper : public vgui::Panel {
public:
	GameThreadHelper();

	void AddCall(std::function<void()> call);

	virtual void OnTick();

private:
	std::queue<std::function<void()>> callQueue;
	std::mutex callQueueLock;
};

extern GameThreadHelper *g_GameThreadHelper;
