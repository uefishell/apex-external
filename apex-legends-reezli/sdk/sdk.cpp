#include "sdk.h"
#include "offsets.h"
#include "../memory/memory.h"
sdk& sdk::here()
{
	static sdk ins;
	return ins;
}



struct Bone {
	BYTE thing[0xCC];
	float x;
	BYTE thing2[0xC];
	float y;
	BYTE thing3[0xC];
	float z;
};

DWORD64 sdk::GetEntity(int Entity)
{
	DWORD64 Entitylist = i_memory::here().base + offsets::init().OFFSET_ENTITYLIST;
	DWORD64 BaseEntity = i_memory::here().read<DWORD64>(Entitylist);
	if (!BaseEntity)
		return NULL;
	return i_memory::here().read<DWORD64>(Entitylist + (Entity << 5));
}

Vector3 sdk::Poisition(uintptr_t entity) {
	return i_memory::here().read<Vector3>(entity + 0x14C);
}

Vector3 sdk::BonePos(uintptr_t Entity, int boneIndex)
{
	Bone bone = {};
	Vector3 vec_bone = Vector3();
	Vector3 pos = Poisition(Entity);

	ULONG64 bone_array = i_memory::here().read<ULONG64>(Entity + offsets::init().OFFSET_BONE);
	ULONG64 bone_location = (boneIndex * 0x30);

	bone = i_memory::here().read<Bone>(bone_array + bone_location);
	vec_bone.x = bone.x + pos.x;
	vec_bone.y = bone.y + pos.y;
	vec_bone.z = bone.z + pos.z;
	return vec_bone;
}

bool sdk::IsTeam(uintptr_t localplayer, DWORD64 entity)
{
	int a = i_memory::here().read<int>(localplayer + 0x44C);
	int b = i_memory::here().read<int>(entity + 0x44C);
	if (a == b)
		return true;

	return false;
}

float sdk::DistanceToLocal(DWORD64 Entity)
{
	return Poisition(Entity).distancee(Poisition(i_apex::here().localplayer));
}

float sdk::LastVisTime(uintptr_t entity)
{
	return  i_memory::here().read<float>(entity + offsets::init().OFFSET_VISIBLE_TIME);
}
float oVisTime[100];
bool sdk::IsVisible(uintptr_t entity, int i) {
	const auto VisCheck = LastVisTime(entity);
	const auto IsVis = VisCheck > oVisTime[i] || VisCheck < 0.f && oVisTime[i] > 0.f;
	oVisTime[i] = VisCheck;
	return IsVis;
}
