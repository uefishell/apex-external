#pragma once
#include "../sdk/sdk.h"
#include <Windows.h>
#include <iostream>

class esp {
public:
	static esp& here();

	void start();
	void esp_manager(DWORD64 pEntity, Matrix m, int index, uintptr_t localplayer);

	bool glow = false;
	bool box = false;
	bool snapline = false;
	bool healthbar = false;
	bool distance2 = false;
	bool cornered = false;
	bool norecoil = false;
	bool skinchanger = false;


	int skinid = 0;
	int skinidlocal = 0;
	int distanceint = 0;
};