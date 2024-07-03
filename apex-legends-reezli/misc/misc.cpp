#include "misc.h"


misc& misc::here()
{
	static misc ins;
	return ins;
}

void misc::exploits()
{
		if (thirdperson)
		{
			i_memory::here().write<uintptr_t>(i_memory::here().base + 0x01cf49c0 + 0x6C, 1);
	    }
		if (!thirdperson)
		{
			i_memory::here().write<uintptr_t>(i_memory::here().base + 0x01cf49c0 + 0x6C, -1);
		}
}

void misc::start()
{
	//soon
	misc::exploits();
}