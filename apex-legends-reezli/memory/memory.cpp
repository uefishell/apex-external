#include "memory.h"
#include "../sdk/offsets.h"
#include <iostream>
#include <TlHelp32.h>

i_memory& i_memory::here()
{
	static i_memory ins;
	return ins;
}

void i_memory::initialize()
{
	pid = GetProcId("r5apex.exe");
	
	if (OpenDriverHandle("r5apex.exe"))
	{


		base = GetBase();

		for (int i = 0; i < 100; i++)
		{
			i_apex::here().localplayer = i_memory::here().read<uintptr_t>(base + offsets::init().OFFSET_LOCAL_ENT);
			i_apex::here().viewrender = i_memory::here().read<uintptr_t>(base + offsets::init().OFFSET_RENDER);
			i_apex::here().entitylist = i_memory::here().read<uintptr_t>(base + offsets::init().OFFSET_ENTITYLIST);
			i_apex::here().viewmatrix = i_memory::here().read<uintptr_t>(i_apex::here().viewrender + offsets::init().OFFSET_MATRIX);
		}


		printf("Game Pid: %d", pid);
		printf("\nGame Base: %p", base);
		printf("\n\nGame Localplayer: %p", i_apex::here().localplayer);
		printf("\nGame ViewRender: %p", i_apex::here().viewrender);
		printf("\nGame EntityList: %p", i_apex::here().entitylist);
		printf("\nGame ViewMatrix: %p", i_apex::here().viewmatrix);

	}
	
	
}

ULONG64 i_memory::GetBase()
{
	communication request = {};
	SecureZeroMemory(&request, sizeof(communication));

	request.key = 0xEBDFEADF;
	request.request = request::getbase;
	request.process_id = pid;
	hooked_function(&request, 0);

	return request.process_base;
}

DWORD i_memory::GetProcId(const char* process_name)
{
	PROCESSENTRY32 pt;
	HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pt.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hsnap, &pt)) {
		do {
			if (!lstrcmpi(pt.szExeFile, process_name)) {
				CloseHandle(hsnap);
				return pt.th32ProcessID;
			}
		} while (Process32Next(hsnap, &pt));
	}
	CloseHandle(hsnap);
	return 0;
}

bool i_memory::OpenDriverHandle(const char* process_name)
{
	
	pid = GetProcId(process_name);
	if (!pid)
	{
		return false;
	}
	else
	{
		HMODULE user32 = LoadLibraryA("user32.dll");
		HMODULE win32u = LoadLibraryA("win32u.dll");

		if (!user32 || !win32u)
		{
			return false;
		}
		else
		{

			FARPROC function_addr = GetProcAddress(win32u, "NtGdiCreateDIBBrush");
			if (!function_addr)
			{
				return false;
			}
			else
			{
				*(void**)&hooked_function = function_addr;

				base = GetBase();
				if (!base)
				{
					return false;
				}
			}
		}
	}

	return true;
}

HWND i_memory::GetProcessWindow(uint32_t pid)
{
	std::pair<HWND, uint32_t> params = { 0, pid };
	BOOL bResult = EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
		auto pParams = (std::pair<HWND, uint32_t>*)(lParam);
		uint32_t processId = 0;

		if (GetWindowThreadProcessId(hwnd, reinterpret_cast<LPDWORD>(&processId)) && processId == pParams->second) {
			SetLastError((uint32_t)-1);
			pParams->first = hwnd;
			return FALSE;
		}

		return TRUE;

		}, (LPARAM)&params);

	if (!bResult && GetLastError() == -1 && params.first)
		return params.first;

	return NULL;
}