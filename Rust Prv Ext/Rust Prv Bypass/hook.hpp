#include "Memory.hpp"

#define QwordSig "\x44\x89\xBC\x24\x00\x00\x00\x00\x03\xC7"," xxxx ? ? ? ? xx"
#define PatternQword "44 89 BC 24 ? ? ? ? 03 C7"

INT64(__stdcall* MmMapIoSpace)(PHYSICAL_ADDRESS a1, SIZE_T a2, MEMORY_CACHING_TYPE a3);

struct Table {
	ULONG ReadRegisrey = 0x9C402428,
		PhysicalRead = 0x9C402420,
		PhysicalWrite = 0x9C402430;
}; Table Ptr;

struct VTable {
	ULONG a1 = 0x4, // add 4
		a2 = 0x0, // add 0
		a3 = 0x7, // push right 0x7 bytes
		a4 = 0x8, // add 8
		a5 = 0x8; //  0x8 -> MmMapIoSpace
}; VTable TableList;
struct Type {
	INT READ = 0,
		WRITE = 1,
		HOOK = 2,
		ALLOC = 3;
}; Type DriverMethod;

class Hook {
private:
public:
	

	static NTSTATUS MmMapIoSpaceHook(PHYSICAL_ADDRESS PhysicalAddress, SIZE_T NumberOfBytes, MEMORY_CACHING_TYPE CacheType)
	{
		KeEnterCriticalRegion();
		if (CacheType == MmNonCached)
		{
			return MmMapIoSpace(PhysicalAddress, NumberOfBytes, CacheType);
			KeLeaveCriticalRegion();
		}else if (CacheType == MmNotMapped)
		{
			return MmMapIoSpace(PhysicalAddress, NumberOfBytes, CacheType);
			KeLeaveCriticalRegion();
		}
		else if (CacheType == DriverMethod.READ)
		{

		}
		else if (CacheType == DriverMethod.WRITE)
		{

		}
		return STATUS_SUCCESS;
	}
	static bool ApplyHook()
	{

	}
};