#include <Windows.h>
#include <iostream>
#include "../main/apex-external.h"
#include "../mapper/drv_image.hpp"
#include "../mapper/vdm.hpp"
#include "../mapper/vdm_ctx.hpp"
#include "driver.h"

inline bool load_driver()
{
	const auto nt_headers = portable_executable::get_nt_headers(drivero.data());

	if (nt_headers->Signature != IMAGE_NT_SIGNATURE)
	{
		std::printf(("[-] invalid nt signature of target file\n"));
		return false;
	}

	std::uint32_t image_size = nt_headers->OptionalHeader.SizeOfImage;
	std::string str = ".data";
	char* cstr = str.data();
	potential_drivers signed_driver = util::get_driver_info("mlx4_bus.sys", cstr, image_size);

	if (signed_driver.number_of_sections == 0)
	{
		std::printf(("[-] failed to get info for potential driver.\n"));
		return false;
	}

	NTSTATUS load_status = driver2::load(signed_driver.file_path, signed_driver.file_name);
	std::uintptr_t signed_module_base = util::get_module_base(signed_driver.file_name.c_str());

	if ((load_status != STATUS_SUCCESS && load_status != 0x1) || !signed_module_base)
	{
		printf(("[-] failed to load [%s] [0x%x]\n"), signed_driver.file_name.c_str(), load_status);
		return false;
	}

	//std::printf(("[+] loaded [vgk.sys] at [%p] with result [0x%x]\n"), signed_driver.file_name, signed_module_base, load_status);

	std::uintptr_t start_map = signed_module_base + signed_driver.section_offset;


	static const auto kernel_base = util::get_module_base(("ntoskrnl.exe"));

	static const auto mm_pte_offset = util::memory::get_mi_pte_address_fn();
	static const auto mm_pde_offset = util::memory::get_mi_pde_address_fn();

	if (!mm_pte_offset || !mm_pde_offset)
	{
		printf(("[-] failed to sig scan pte functions\n"));
		return false;
	}

	const auto [drv_handle, drv_key] = vdm::load_drv();
	if (!drv_handle || drv_key.empty())
	{
		
		return true;
	}

	//std::printf(("[+] vulnerable driver loaded [%s]\n"), drv_key.c_str());

	vdm::vdm_ctx kernel{};

	if (!kernel.clear_piddb_cache(drv_key, util::get_file_header((void*)vdm::raw_driver)->TimeDateStamp))
	{
		std::printf(("[-] failed to clear piddbcache table\n"));
		vdm::unload_drv(drv_handle, drv_key);

		return false;
	}

	for (std::uintptr_t start = start_map; start < start_map + image_size; start += PAGE_4KB)
	{
		auto pde = kernel.syscall<MiGetPteAddress>(reinterpret_cast<void*>(mm_pde_offset), start);

		::pde new_pde = kernel.rkm<::pde>(pde);

		//	std::printf(("[+] manipulating page table [%p] large page [%x]\n"), start, new_pde.large_page);

		if (new_pde.large_page)
		{
			new_pde.nx = 0;
			new_pde.rw = 1;

			kernel.wkm<::pde>(pde, new_pde);
		}
		else
		{
			auto pte = kernel.syscall<MiGetPteAddress>(reinterpret_cast<void*>(mm_pte_offset), start);

			::pte new_pte = kernel.rkm<::pte>(pte);

			new_pte.nx = 0;
			new_pte.rw = 1;

			kernel.wkm<::pte>(pte, new_pte);
		}
	}

	void* local_image_base = VirtualAlloc(0, image_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	memcpy(local_image_base, drivero.data(), nt_headers->OptionalHeader.SizeOfHeaders);

	//std::printf(("\n[+] local mapped image base [%p]\n"), local_image_base);

	PIMAGE_SECTION_HEADER current_image_section = IMAGE_FIRST_SECTION(nt_headers);

	for (auto i = 0; i < nt_headers->FileHeader.NumberOfSections; ++i)
	{
		void* local_section = reinterpret_cast<void*>(reinterpret_cast<uint64_t>(local_image_base) + current_image_section[i].VirtualAddress);
		memcpy(local_section, reinterpret_cast<void*>(reinterpret_cast<uint64_t>(drivero.data()) + current_image_section[i].PointerToRawData), current_image_section[i].SizeOfRawData);

		//std::printf(("[+] mapping section [%s] [%p] > [%p] [0x%x]\n"), current_image_section[i].Name, local_section, start_map + current_image_section[i].VirtualAddress, current_image_section[i].SizeOfRawData);
	}


	portable_executable::relocate_image_by_delta(portable_executable::get_relocations(local_image_base), start_map - nt_headers->OptionalHeader.ImageBase);

	if (!portable_executable::resolve_imports(portable_executable::get_imports(local_image_base)))
	{
		std::printf(("[-] failed to resolve imports\n"));
		vdm::unload_drv(drv_handle, drv_key);
		return false;
	}

	RtlZeroMemory(local_image_base, nt_headers->OptionalHeader.SizeOfHeaders);
	kernel.wkm(reinterpret_cast<void*>(start_map), local_image_base, image_size);

	std::uintptr_t entry_point = start_map + nt_headers->OptionalHeader.AddressOfEntryPoint;

	//std::printf(("\n[+] mapped entry point [%p]\n"), entry_point);

	NTSTATUS result = kernel.syscall<EntryCall>(reinterpret_cast<void*>(entry_point), start_map, nt_headers->OptionalHeader.SizeOfImage);

	//	std::printf(("[+] driver entry returned [0x%x]\n"), result);

	if (!vdm::unload_drv(drv_handle, drv_key))
	{
		std::printf(("[-] unable to unload vulnerable driver\n"));
		return false;
	}

	std::printf(("\n[+] mapped [%p]\n"), entry_point);
	return true;
}
auto main() -> int
{

	
	

	if (load_driver())
	{
		printf("\nDriver: Loaded");
		i_apex::here().start();
	}
	
	system("pause>nul");

	
}