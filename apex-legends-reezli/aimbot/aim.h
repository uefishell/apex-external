#pragma once
#include "../sdk/sdk.h"
#include <Windows.h>
#include <iostream>
#include "../memory/memory.h"

struct AimContext {
	int FOV;
	int FOV2 = 25;
	int aSmoothAmount;

	int crosshairX = GetSystemMetrics(SM_CXSCREEN) / 2;
	int crosshairY = GetSystemMetrics(SM_CYSCREEN) / 2;

	int entX = 0;
	int entY = 0;

	int closestX = 0;
	int closestY = 0;

	int aX = 0;
	int aY = 0;

	float entNewVisTime = 0;
	float entOldVisTime[100];
	int visCooldownTime[100];
};

class aim {
public:
	static aim& here();

	void start();
	void aim_manager(uintptr_t pEntity, uintptr_t pLocal, Matrix m, AimContext* Ctx, int index);
	void aim_move(AimContext* Ctx);

	bool enable = false;
	bool drawfov = false;
	bool visiblecheck = false;
	bool norecoil;

	int smooth = 0;
	int distance = 0;
	int fov = 0;

	int boneid = 0;
	int aimkey = 0;

};


