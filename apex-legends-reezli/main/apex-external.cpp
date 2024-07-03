#include "apex-external.h"
#include "../sdk/offsets.h"

i_apex& i_apex::here()
{
	static i_apex ins;
	return ins;
}

void i_apex::logs()
{

}

void i_apex::start()
{
	i_memory::here().initialize();
	overlay::init().initialize();
}

void overlay::render()
{

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	if (aim::here().drawfov)
	{
		ImGui::GetOverlayDrawList()->AddCircle(ImVec2(ScreenCenterX, ScreenCenterY), aim::here().fov, IM_COL32_WHITE, 10000, 1);
	}
	
	misc::here().start();
	Matrix m = i_memory::here().read<Matrix>(i_apex::here().viewmatrix);

	AimContext aimCtx; aimCtx.closestX = 50000; aimCtx.closestY = 50000; aimCtx.FOV = aim::here().fov; aimCtx.aSmoothAmount = aim::here().smooth;
	for (int i = 0; i < 100; i++)
	{
		DWORD64 Entity = sdk::here().GetEntity(i);

		DWORD64 EntityHandle = i_memory::here().read<DWORD64>(Entity + 0x589);

		DWORD64 EntityShield = i_memory::here().read<DWORD64>(Entity + 0x170);

		std::string Identifier = i_memory::here().read<std::string>(EntityHandle);
		LPCSTR IdentifierC = Identifier.c_str();
		if (strcmp(IdentifierC, "player"))
		{
			esp::here().esp_manager(Entity, m, i, i_apex::here().localplayer);
		}

		if (aim::here().enable)
		{

			if (aim::here().visiblecheck)
			{
				if (sdk::here().IsVisible(Entity, i))
				{
					aim::here().aim_manager(Entity, i_apex::here().localplayer, m, &aimCtx, i);
				}
			}
			else
			{
				aim::here().aim_manager(Entity, i_apex::here().localplayer, m, &aimCtx, i);
			}
			
			
			
		}
	}
	if (aim::here().enable)
	{
		aim::here().aim_move(&aimCtx);
	}
	DrawString(16, 5, 5, false, false, "external overlay - northway");

	
	if ((GetAsyncKeyState)(VK_F5) & 1)
	{
		showmenu = !showmenu;
	}

	if (showmenu)
	{
		menu();

	}

	ImGui::GetIO().MouseDrawCursor = showmenu;
	ImGui::GetIO().WantCaptureKeyboard = showmenu;
	ImGui::EndFrame();
	p_Device->SetRenderState(D3DRS_ZENABLE, false);
	p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	p_Device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
	p_Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	if (p_Device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		p_Device->EndScene();
	}

	HRESULT result = p_Device->Present(NULL, NULL, NULL, NULL);

	if (result == D3DERR_DEVICELOST && p_Device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		p_Device->Reset(&d3dpp);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}