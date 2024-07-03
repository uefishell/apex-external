#include "overlay.h"


overlay& overlay::init()
{
	static overlay ins;
	return ins;
}

HRESULT overlay::init_wndparams(HWND hwnd)
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
		exit(3);

	ZeroMemory(&p_Params, sizeof(p_Params));
	p_Params.Windowed = TRUE;
	p_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_Params.hDeviceWindow = hwnd;
	p_Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	p_Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_Params.BackBufferWidth = Width;
	p_Params.BackBufferHeight = Height;
	p_Params.EnableAutoDepthStencil = TRUE;
	p_Params.AutoDepthStencilFormat = D3DFMT_D16;
	p_Params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(p_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_Params, 0, &p_Device))) {
		p_Object->Release();
		exit(4);
	}

	ImGui::CreateContext();


	//D3DXCreateTextureFromFileInMemoryEx(p_Device, &skid, sizeof(skid), 407, 695, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &unknowncheatslogo);
	//ImGui::GetIO().Fonts->AddFontDefault();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = NULL;
	ImFontConfig font_config;
	font_config.OversampleH = 1; //or 2 is the same
	font_config.OversampleV = 1;
	font_config.PixelSnapH = 1;
	static const ImWchar ranges[] =
	{
		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0x0400, 0x044F, // Cyrillic
		0,
	};


	//ico_combo2 = io.Fonts->AddFontFromMemoryTTF(&icon, sizeof icon, 19, NULL, io.Fonts->GetGlyphRangesCyrillic());
	//LexendRegular = io.Fonts->AddFontFromMemoryTTF(LexendRegularfont, sizeof(LexendRegularfont), 17, NULL, io.Fonts->GetGlyphRangesCyrillic());
	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	//global3 = io.Fonts->AddFontFromMemoryTTF(segoui, sizeof(segoui), 17, NULL, io.Fonts->GetGlyphRangesCyrillic());

	//io.Fonts->AddFontFromMemoryCompressedBase85TTF(RobotoLight_compressed_data_base85, 18.0F);


	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX9_Init(p_Device);
	return S_OK;
}

void overlay::cleanup_d3d()
{
	if (p_Device != NULL) {
		p_Device->EndScene();
		p_Device->Release();
	}
	if (p_Object != NULL) {
		p_Object->Release();
	}
}

void set_window_target()
{
	while (true) {
		overlay::init().GameWnd = i_memory::here().GetProcessWindow(i_memory::here().pid);
		if (overlay::init().GameWnd) {
			ZeroMemory(&overlay::init().GameRect, sizeof(overlay::init().GameRect));
			GetWindowRect(overlay::init().GameWnd, &overlay::init().GameRect);
			DWORD dwStyle = GetWindowLong(overlay::init().GameWnd, GWL_STYLE);
			if (dwStyle & WS_BORDER)
			{
				overlay::init().GameRect.top += 32;
				overlay::init().Height -= 39;
			}
			overlay::init().ScreenCenterX = overlay::init().Width / 2;
			overlay::init().ScreenCenterY = overlay::init().Height / 2;
			MoveWindow(overlay::init().MyWnd, overlay::init().GameRect.left, overlay::init().GameRect.top, overlay::init().Width, overlay::init().Height, true);
		}
	}
}

void overlay::setup_window()
{
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)set_window_target, 0, 0, 0);
	WNDCLASSEXA wcex = {
		sizeof(WNDCLASSEXA),
		0,
		DefWindowProcA,
		0,
		0,
		nullptr,
		LoadIcon(nullptr, IDI_APPLICATION),
		LoadCursor(nullptr, IDC_ARROW),
		nullptr,
		nullptr,
		("Chrome"),
		LoadIcon(nullptr, IDI_APPLICATION)
	};

	RECT Rect;
	GetWindowRect(GetDesktopWindow(), &Rect);

	RegisterClassExA(&wcex);

	MyWnd = CreateWindowExA(NULL, ("Chrome"), ("Chrome"), WS_POPUP, Rect.left, Rect.top, Rect.right, Rect.bottom, NULL, NULL, wcex.hInstance, NULL);
	SetWindowLong(MyWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
	SetLayeredWindowAttributes(MyWnd, RGB(0, 0, 0), 255, LWA_ALPHA);

	MARGINS margin = { -1 };
	DwmExtendFrameIntoClientArea(MyWnd, &margin);

	ShowWindow(MyWnd, SW_SHOW);
	UpdateWindow(MyWnd);
}

void overlay::style()
{
	ImGui::StyleColorsDark();
}

void overlay::TextCentered(std::string text) {
	auto windowWidth = ImGui::GetWindowSize().x;
	auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

	ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
	ImGui::Text(text.c_str());
}

void overlay::menu()
{
	
	static int tab = 0;
	ImGui::SetNextWindowSize({ 600, 365 }, ImGuiCond_Once);
	ImGui::Begin("meow", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
	{
		ImGui::SetCursorPos({ 15, 40 });
		ImGui::BeginChild("Tabs", { 125,300 });
		{
			ImGui::SetCursorPos({ 15,15 });
			if (ImGui::Button("Aimbot", { 85, 25 }))
			{
				tab = 0;
			}
			ImGui::SetCursorPos({ 15, 50 });
			if (ImGui::Button("Visuals", { 85, 25 }))
			{
				tab = 1;
			}
			ImGui::SetCursorPos({ 15, 85 });
			if (ImGui::Button("Misc", { 85, 25 }))
			{
				tab = 2;
			}
			ImGui::SetCursorPos({ 15, 120 });
			if (ImGui::Button("Config", { 85, 25 }))
			{
				tab = 3;
			}
		}
		ImGui::EndChild();

		if (tab == 0)
		{
			ImGui::SetCursorPos({ 155, 40 });
			ImGui::BeginChild("General", { 210,300 });
			{
				TextCentered("General");
				ImGui::Separator();

				ImGui::SetCursorPosX(15);
				ImGui::Spacing();
				ImGui::Checkbox("Enable", &aim::here().enable);
				ImGui::Checkbox("Draw Fov", &aim::here().drawfov);
				ImGui::Checkbox("Visible Check", &aim::here().visiblecheck);
				ImGui::Checkbox("No Recoil", &esp::here().norecoil);

				ImGui::SetCursorPosX(15);
				ImGui::Spacing();
				const char* aimkeys[]{ "XBUTTON2","XBUTTON1", "LBUTTON", "RBUTTON"};
				ImGui::Combo("Aim Key", &aim::here().aimkey, aimkeys, sizeof(aimkeys));

			}
			ImGui::EndChild();

			ImGui::SetCursorPos({ 375, 40 });
			ImGui::BeginChild("Other", { 210,300 });
			{
				TextCentered("Other");
				ImGui::Separator();

				ImGui::SetCursorPosX(15);
				ImGui::Spacing();
				ImGui::SliderInt("Smooth", &aim::here().smooth, 0, 15);
				ImGui::SetCursorPosX(15);
				ImGui::Spacing();
				ImGui::SliderInt("Fov", &aim::here().fov, 0, 500);

				ImGui::SetCursorPosX(15);
				ImGui::Spacing();
				const char* bone[]{ "Head","Neck", "Body"};
				ImGui::Combo("Bone Select", &aim::here().boneid, bone, sizeof(bone));
			}
			ImGui::EndChild();
		}

		if (tab == 1)
		{
			ImGui::SetCursorPos({ 155, 40 });
			ImGui::BeginChild("Esp", { 210,300 });
			{
				TextCentered("Esp");
				ImGui::Separator();

				ImGui::SetCursorPosX(15);
				ImGui::Spacing();
				ImGui::Checkbox("Glow", &esp::here().glow);
				ImGui::SetCursorPosX(15);
				ImGui::Spacing();
				ImGui::Checkbox("Box", &esp::here().box);
				ImGui::SetCursorPosX(15);
				ImGui::Spacing();
				ImGui::Checkbox("Health Bar", &esp::here().healthbar);
				ImGui::SetCursorPosX(15);
				ImGui::Spacing();
				ImGui::Checkbox("Snapline", &esp::here().snapline);
				ImGui::SetCursorPosX(15);
				ImGui::Spacing();
				ImGui::Checkbox("Distance", &esp::here().distance2);
			}
			ImGui::EndChild();

			ImGui::SetCursorPos({ 375, 40 });
			ImGui::BeginChild("Esp Settings", { 210,300 });
			{
				TextCentered("Settings");
				ImGui::Separator();

				ImGui::SetCursorPosX(15);
				ImGui::Spacing();
				ImGui::SliderInt("Distance", &esp::here().distanceint,0, 500);
			}
			ImGui::EndChild();
		}
		if (tab == 2)
		{
			ImGui::SetCursorPos({ 155, 40 });
			ImGui::BeginChild("Exploits", { 210,300 });
			{
				TextCentered("Some Stuff");
				ImGui::Separator();

				ImGui::SetCursorPosX(15);
				ImGui::Spacing();
				ImGui::Checkbox("Thirdperson", &misc::here().thirdperson);
				ImGui::Checkbox("Skin Changer", &esp::here().skinchanger);
			}
			ImGui::EndChild();

			ImGui::SetCursorPos({ 375, 40 });
			ImGui::BeginChild("Sett", { 210,300 });
			{
				TextCentered("Some Stuff V2");
				ImGui::Separator();


				ImGui::SetCursorPosX(15);
				ImGui::Spacing();
				ImGui::SliderInt("Skin Id", &esp::here().skinid, 0, 20);

				ImGui::SetCursorPosX(15);
				ImGui::Spacing();
				ImGui::SliderInt("Skin Id Local", &esp::here().skinidlocal, 0, 20);
			}
			ImGui::EndChild();
		}
	}
	ImGui::End();
}

std::wstring MBytesToWString(const char* lpcszString)
{
	int len = strlen(lpcszString);
	int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, NULL, 0);
	wchar_t* pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, (LPWSTR)pUnicode, unicodeLen);
	std::wstring wString = (wchar_t*)pUnicode;
	delete[] pUnicode;
	return wString;
}

std::string WStringToUTF8(const wchar_t* lpwcszWString)
{
	char* pElementText;
	int iTextLen = ::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, NULL, 0, NULL, NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, (iTextLen + 1) * sizeof(char));
	::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, pElementText, iTextLen, NULL, NULL);
	std::string strReturn(pElementText);
	delete[] pElementText;
	return strReturn;
}

void overlay::DrawString(float fontSize, int x, int y, bool bCenter, bool stroke, const char* pText, ...)
{
	va_list va_alist;
	char buf[1024] = { 0 };
	va_start(va_alist, pText);
	_vsnprintf_s(buf, sizeof(buf), pText, va_alist);
	va_end(va_alist);
	std::string text = WStringToUTF8(MBytesToWString(buf).c_str());
	if (bCenter)
	{
		ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
		x = x - textSize.x / 4;
		y = y - textSize.y;
	}
	if (stroke)
	{
		ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x + 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x - 1, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x + 1, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x - 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
	}
	ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImColor(255,255,255)), text.c_str());
}

void overlay::signal()
{
	ZeroMemory(&Message, sizeof(MSG));
	style();
	static RECT old_rc;
	while (Message.message != WM_QUIT)
	{
		if (PeekMessage(&Message, MyWnd, 0, 0, PM_REMOVE)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		HWND hwnd_active = GetForegroundWindow();


		if (hwnd_active == GameWnd) {
			HWND hwndtest = GetWindow(hwnd_active, GW_HWNDPREV);
			SetWindowPos(MyWnd, hwndtest, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		}
		
		RECT rc;
		POINT xy;

		ZeroMemory(&rc, sizeof(RECT));
		ZeroMemory(&xy, sizeof(POINT));
		GetClientRect(GameWnd, &rc);
		ClientToScreen(GameWnd, &xy);
		rc.left = xy.x;
		rc.top = xy.y;

		ImGuiIO& io = ImGui::GetIO();
		io.ImeWindowHandle = GameWnd;
		io.DeltaTime = 1.0f / 60.0f;

		POINT p;
		GetCursorPos(&p);
		io.MousePos.x = p.x - xy.x;
		io.MousePos.y = p.y - xy.y;
		if (GetAsyncKeyState(0x1)) {
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else io.MouseDown[0] = false;

		if (rc.left != old_rc.left || rc.right != old_rc.right || rc.top != old_rc.top || rc.bottom != old_rc.bottom) {

			old_rc = rc;

			Width = rc.right;
			Height = rc.bottom;

			p_Params.BackBufferWidth = Width;
			p_Params.BackBufferHeight = Height;
			SetWindowPos(MyWnd, (HWND)0, xy.x, xy.y, Width, Height, SWP_NOREDRAW);
			p_Device->Reset(&p_Params);
		}
		overlay::init().render();

		Sleep(10);
	}
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	overlay::init().cleanup_d3d();
	DestroyWindow(overlay::init().MyWnd);

}

void overlay::DrawLine(const ImVec2& x, const ImVec2 y, ImU32 color, const FLOAT width)
{
	ImGui::GetOverlayDrawList()->AddLine(x, y, color, width);
}

void overlay::initialize()
{
	//xyzus sigma
	setup_window();
	init_wndparams(MyWnd);
	Sleep(2000);
	signal();
}