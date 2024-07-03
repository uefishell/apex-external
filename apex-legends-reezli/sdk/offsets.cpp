#include "offsets.h"

offsets& offsets::init()
{
	static offsets ins;
	return ins;
}
