#pragma once

#include <Windows.h>
#include <iostream>
#include <string>
#include "fonts.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "../memory/memory.h"
#include "color.h"

class overlay {
public:
	static overlay& init();

	HRESULT init_wndparams(HWND hwnd);
	void cleanup_d3d();
	void setup_window();
	void style();
	void menu();
	void signal();
	void render();
	void initialize();

	IDirect3D9Ex* p_Object = NULL;
	IDirect3DDevice9Ex* p_Device = NULL;
	D3DPRESENT_PARAMETERS p_Params = { NULL };
	HWND MyWnd = NULL;
	HWND GameWnd = NULL;
	MSG Message = { NULL };
	RECT GameRect = { NULL };
	D3DPRESENT_PARAMETERS d3dpp;
	ULONG Width = GetSystemMetrics(SM_CXSCREEN);
	ULONG Height = GetSystemMetrics(SM_CYSCREEN);
	DWORD ScreenCenterX;
	DWORD ScreenCenterY;
	ImFont* global3;
	ImFont* LexendRegular;
	void TextCentered(std::string text);
	void DrawString(float fontSize, int x, int y, bool bCenter, bool stroke, const char* pText, ...);
	void DrawLine(const ImVec2& x, const ImVec2 y, ImU32 color, const FLOAT width);
	bool showmenu = true;

};


