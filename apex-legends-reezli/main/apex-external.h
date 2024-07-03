#pragma once
#include <Windows.h>
#include <string>
#include "../memory/memory.h"
#include "../overlay/overlay.h"
#include "../misc/misc.h"
#include "../esp/esp.h"
#include "../aimbot/aim.h"
class i_apex {
public:
	static i_apex& here();

	void start();
	void logs();

	uintptr_t localplayer;
	uintptr_t viewrender;
	uintptr_t entitylist;
	uintptr_t viewmatrix;

};