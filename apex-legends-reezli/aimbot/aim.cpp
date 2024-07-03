#include "aim.h"
#include "../sdk/offsets.h"
aim& aim::here()
{
	static aim ins;
	return ins;
}

inline int isKnocked2(uintptr_t entity) {
	return i_memory::here().read<int>(entity + offsets::init().OFFSET_BLEED_OUT_STATE);
}

inline int iTeam(uintptr_t entity) {
	return i_memory::here().read<int>(entity + offsets::init().OFFSET_TEAM);
}

void aim::start()
{

}

void aim::aim_manager(uintptr_t pEntity, uintptr_t pLocal, Matrix m, AimContext* Ctx, int index)
{
	int hit;
	if (boneid == 0)
		hit == 8;
	else if (boneid == 1)
		hit == 7;
	else if (boneid == 2)
		hit == 5;
	
	Vector3 entAimPos = sdk::here().BonePos(pEntity, boneid);
	uintptr_t entitylist = i_memory::here().read<uintptr_t>(i_memory::here().base + offsets::init().OFFSET_ENTITYLIST);

	Vector3 w2sEntAimPos = entAimPos.WorldToScreen(m); if (w2sEntAimPos.z <= 0.f) return;

	int entX = w2sEntAimPos.x;
	int entY = w2sEntAimPos.y;

	Ctx->entNewVisTime = i_memory::here().read<float>(pEntity + offsets::init().OFFSET_VISIBLE_TIME);
	int entKnockedState = isKnocked2(pEntity);
	int playerTeamID = iTeam(pLocal);
	int entTeamID = iTeam(pEntity);

	if (entTeamID != playerTeamID) {

		if (Ctx->entNewVisTime != Ctx->entOldVisTime[index])
		{
			Ctx->visCooldownTime[index] = 24;

			if (entKnockedState == 0)
			{

				if (abs(Ctx->crosshairX - entX) < abs(Ctx->crosshairX - Ctx->closestX) && abs(Ctx->crosshairX - entX) < Ctx->FOV && abs(Ctx->crosshairY - entY) < abs(Ctx->crosshairY - Ctx->closestY) && abs(Ctx->crosshairY - entY) < Ctx->FOV)
				{
					Ctx->closestX = entX;
					Ctx->closestY = entY;
				}
			}
			Ctx->entOldVisTime[index] = Ctx->entNewVisTime;
		}
		if (Ctx->visCooldownTime[index] >= 0) Ctx->visCooldownTime[index] -= 1;
	}
}

void aim::aim_move(AimContext* Ctx) {
	int aimkey2 = 0;
	if (aimkey == 0)
	{
		aimkey2 = 0x06;
	}
	if (aimkey == 1)
	{
		aimkey2 = 0x05;
	}
	if (aimkey == 2)
	{
		aimkey2 = 0x01;
	}
	if (aimkey == 3)
	{
		aimkey2 = 0x02;
	}

	if (Ctx->closestX != 50000 && Ctx->closestY != 50000)
	{
		if (GetAsyncKeyState(aimkey2))
		{
			Ctx->aX = (Ctx->closestX - Ctx->crosshairX) / Ctx->aSmoothAmount;
			Ctx->aY = (Ctx->closestY - Ctx->crosshairY) / Ctx->aSmoothAmount;

			CURSORINFO ci = { sizeof(CURSORINFO) };
			if (GetCursorInfo(&ci))
			{
				if (ci.flags == 0)
					mouse_event(MOUSEEVENTF_MOVE, Ctx->aX, Ctx->aY, 0, 0);
			}
		}
	}
}


