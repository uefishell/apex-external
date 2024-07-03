#pragma once
#include <iostream>
#include <Windows.h>


class offsets {
public:
	
	static offsets& init();
	// BOX
	int OFFSET_MATRIX = 0x11a210; // ViewMatrix
	int OFFSET_RENDER = 0x786f390; // ViewRender
	int OFFSET_ENTITYLIST = 0x1d0fe58; // cl_entitylist
	int OFFSET_ORIGIN = 0x14C; // m_vecAbsOrigin
	int OFFSET_BONE = 0x0e98 + 0x48;


	// ENTITY
	int OFFSET_LOCAL_ENT = 0x20C1138; // LocalPlayer

	int OFFSET_TEAM = 0x044c; // m_iTeamNum
	int OFFSET_HEALTH = 0x043c; // m_iHealth
	int OFFSET_SHEILD = 0x0170; // m_shieldHealth

	int OFFSET_LIFE_STATE = 0x0798; // m_lifeState
	int OFFSET_BLEED_OUT_STATE = 0x2740; // m_bleedoutState
	int OFFSET_VISIBLE_TIME = 0x1A78; // m_visibletime


	// GLOW
	int GLOW_CONTEXT = 0x3C8;
	int GLOW_VISIBLE_TYPE = 0x3D0;
	int GLOW_TYPE = 0x2C4;
	int GLOW_COLOR = 0x1d0;
};
