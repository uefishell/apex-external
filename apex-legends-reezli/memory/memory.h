#pragma once
#include <Windows.h>
#include <string>
#include "../main/apex-external.h"

class i_memory {
public:
	static i_memory& here();

	void initialize();

	enum request {
		getbase = 0,
		reading = 1,
		writing = 2
	};

	struct communication {
		request request;
		DWORD process_id;
		DWORD key;
		ULONG64 process_base;

		void* output;
		void* buffer_address;
		uintptr_t address;
		size_t size;
	};

	void(__fastcall* hooked_function)(void* a1, void* a2);
	DWORD pid = 0;
	uint64_t base = 0;

	template <class T>
	inline T read(uint64_t address, size_t size = sizeof(T))
	{
		T response{};

		communication request = {};
		SecureZeroMemory(&request, sizeof(communication));

		request.key = 0xEBDFEADF;
		request.request = request::reading;
		request.process_id = pid;
		request.address = address;
		request.output = &response;
		request.size = size;
		hooked_function(&request, 0);

		return response;
	}

	template <typename T>
	inline bool write(uintptr_t address, T value, size_t size = sizeof(T))
	{
		communication request = {};
		SecureZeroMemory(&request, sizeof(communication));

		request.key = 0xEBDFEADF;
		request.request = request::writing;
		request.process_id = pid;
		request.address = address;
		request.size = size;
		request.buffer_address = (void*)&value;
		hooked_function(&request, 0);

		return true;
	}
	void Attach(uint32_t pid) { this->pid = pid; }
	inline ULONG64 GetBase();
	DWORD GetProcId(const char* process_name);
	inline bool OpenDriverHandle(const char* process_name);
	HWND GetProcessWindow(uint32_t pid);
};

inline  i_memory* com = new i_memory();