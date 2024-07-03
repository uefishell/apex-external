#include "esp.h"

#include "../memory/memory.h"
#include "../sdk/offsets.h"



esp& esp::here()
{
	static esp ins;
	return ins;
}

struct GlowMode
{
	int8_t GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
};

void DrawCorneredBox(int X, int Y, int W, int H, ImVec4 color, int thickness)
{
	float lineW = (W / 3);
	float lineH = (H / 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImGui::ColorConvertFloat4ToU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImGui::ColorConvertFloat4ToU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImGui::ColorConvertFloat4ToU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImGui::ColorConvertFloat4ToU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImGui::ColorConvertFloat4ToU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImGui::ColorConvertFloat4ToU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImGui::ColorConvertFloat4ToU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImGui::ColorConvertFloat4ToU32(color), thickness);
}

static void DrawFilledRectImGui(int x, int y, int w, int h, ImVec4 color)
{
	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(color), 0, 0);
}

void DrawBox(float X, float Y, float W, float H, const ImU32& color, int thickness)
{
	ImGui::GetForegroundDrawList()->AddRect(ImVec2(X + 1, Y + 1), ImVec2(((X + W) - 1), ((Y + H) - 1)), ImGui::GetColorU32(color), thickness);
	ImGui::GetForegroundDrawList()->AddRect(ImVec2(X, Y), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
}

int isKnocked(uintptr_t entity) {
	return i_memory::here().read<int>(entity + offsets::init().OFFSET_BLEED_OUT_STATE);
}

inline DWORD64 GetWeapon(DWORD64 entity)
{
	DWORD64 WeaponHandle = i_memory::here().read<DWORD64>(entity + 0x1a1c);
	WeaponHandle &= 0xffff;
	return i_memory::here().read<DWORD64>(i_memory::here().base + offsets::init().OFFSET_ENTITYLIST + (WeaponHandle << 5));
}

void esp::esp_manager(DWORD64 pEntity, Matrix m, int index, uintptr_t localplayer)
{
	Vector3 entHead = sdk::here().BonePos(pEntity, 8); // get head position
	Vector3 w2sHead = entHead.WorldToScreen(m); if (w2sHead.z <= 0.f) return;


	Vector3 entPos = sdk::here().Poisition(pEntity);
	Vector3 w2sPos = entPos.WorldToScreen(m); if (w2sPos.z <= 0.f) return;

	float height;
	float width;
	
	float BoxWidthValue = 0.550;

	height = abs(abs(w2sHead.y) - abs(w2sPos.y));
	width = height / 2.f;

	
	DWORD64 Health = i_memory::here().read<DWORD64>(pEntity + offsets::init().OFFSET_HEALTH);
	

	int G = (255 * Health / 100);
	int R = 255 - G;
	RGBA healthcol = { R, G, 0, 255 };

	int distance = (int)sdk::here().DistanceToLocal(pEntity) / 50.f;
	std::string DistanceESP = "" + std::to_string(distance) + "m";

	if (Health <= 0) return;

	if (distance <= distanceint)
	{
		if (sdk::here().IsTeam(localplayer, pEntity)) return;


		if (!isKnocked(pEntity))
		{

			if (box)
			{
				DrawBox(w2sHead.x - (width / 2), w2sHead.y, width, height, ImColor(157, 176, 242), 1);
			}
			if (snapline)
			{
				overlay::init().DrawLine(ImVec2(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN)), ImVec2(w2sPos.x, w2sPos.y), ImColor(255, 255, 255), 1.f);
			}
			if (distance2)
			{
				overlay::init().DrawString(12, w2sPos.x, (w2sPos.y + 15), &Col.white, true, DistanceESP.c_str());
			}
			if (healthbar)
			{
				float entHp = Health;
				if (entHp > 101) entHp = 100;
				float HealthHeightCalc = ((float)entHp / 100) * (float)height;

				DrawFilledRectImGui(w2sHead.x - (width / 2), w2sHead.y, 2, height, ImColor(30, 30, 30, 225));
				DrawFilledRectImGui(w2sHead.x - (width / 2), w2sHead.y, 2, HealthHeightCalc, ImColor(80, 220, 100, 255));
			}
			

			if (norecoil)
			{
				Vector3 viewangles = i_memory::here().read<Vector3>(i_apex::here().localplayer + 0x25ac - 0x14);
				Vector3 aimpunch = i_memory::here().read<Vector3>(i_apex::here().localplayer + 0x24b0);
				Vector3 newAngle = viewangles + (sdk::here().oldPunch - aimpunch);
				newAngle.Normalize();
				i_memory::here().write<Vector2>(i_apex::here().localplayer + 0x25ac - 0x14, { newAngle.x, newAngle.y });
				i_memory::here().write<Vector2>(i_apex::here().localplayer + 0x25ac - 0x14, { newAngle.x, newAngle.y });
				i_memory::here().write<Vector2>(i_apex::here().localplayer + 0x25ac - 0x14, { newAngle.x, newAngle.y });
				sdk::here().oldPunch = aimpunch;
			}
			if (skinchanger)
			{
				if (i_memory::here().read<int>(GetWeapon(i_apex::here().localplayer) + 0x0e54) != static_cast<int>(skinid)) {
					i_memory::here().write<int>(GetWeapon(i_apex::here().localplayer) + 0x0e54, static_cast<int>(skinid));
				}
				if (i_memory::here().read<int>(i_apex::here().localplayer + 0x0e54) != static_cast<int>(skinidlocal));
				i_memory::here().write<int>(i_apex::here().localplayer + 0x0e54, static_cast<int>(skinidlocal));
			}
			if (cornered)
			{

			}

			if (glow)
			{

				i_memory::here().write<int>(pEntity + 0x3c8, glow);
				i_memory::here().write<int>(pEntity + 0x3d0, 2);
				i_memory::here().write<GlowMode>(pEntity + 0x2c4, { 101,101,46,110 });
				i_memory::here().write<float>(pEntity + 0x1D0, 178);// g
				i_memory::here().write<float>(pEntity + 0x1D4, 176);// g
				i_memory::here().write<float>(pEntity + 0x1D8, 242);// g


			}
		}
	}

	

	
}
			
		
	


void esp::start()
{

}
