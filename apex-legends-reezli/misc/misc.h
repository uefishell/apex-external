#pragma once

#include <Windows.h>
#include <iostream>
#include "../memory/memory.h"

class misc {
public:
	static misc& here();

	void start();

	void exploits();

	bool thirdperson = false;
};