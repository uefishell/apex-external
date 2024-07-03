#pragma once

#include <Windows.h>
#include <iostream>
#include "vector.h"


class sdk {
public:
	static sdk& here();
	DWORD64 GetEntity(int Entity);
	Vector3 BonePos(uintptr_t Entity, int boneIndex);
	Vector3 Poisition(uintptr_t Entity);
	bool IsTeam(uintptr_t localplayer, DWORD64 entity);
	float DistanceToLocal(DWORD64 Entity);
	float LastVisTime(uintptr_t entity);
	bool IsVisible(uintptr_t entity, int i);
	Vector3 oldPunch = { 0.f, 0.f, 0.f };
};
