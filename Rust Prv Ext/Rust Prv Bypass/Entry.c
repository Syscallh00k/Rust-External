//Shared Memory 
#include <ntddk.h>
#include <ntdef.h>

//cpuz141.sys MmMapIoSpace / MmUnmapIoSpace
//
//Device Name : "\\.\cpuz141"
//Read Register IOCTL : 0x9C402428
//Physical Read : 0x9C402420
//Physical Write : 0x9C402430
//
//Notes : CVE - 2017 - 15303
DRIVER_INITIALIZE DriverEntry;

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT driver_object, _In_ PUNICODE_STRING Registry_Path)
{
	UNREFERENCED_PARAMETER(driver_object);
	UNREFERENCED_PARAMETER(Registry_Path);
	PDRIVER_OBJECT pDriverObject;
	PUNICODE_STRING DriverName, SymName;

	RtlInitUnicodeString(&DriverName, "\\Device\\Crt");
	RtlInitUnicodeString(&SymName, "\\DosDevice\\Crt");

	return STATUS_SUCCESS;

}