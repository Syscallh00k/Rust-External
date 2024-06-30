#include "Imports.hpp"


class Kernel {
public:
	class Driver {
	public:
		UNICODE_STRING DriverName, SymbosName;
		PDRIVER_OBJECT PDevice;
		NTSTATUS CreateCall(PDRIVER_OBJECT pDriverObject, IRP irp)
		{
			irp.IoStatus.Status = STATUS_SUCCESS;
			irp.IoStatus.Information = 0;
			IofCompleteRequest(&irp, IO_NO_INCREMENT);
			return STATUS_SUCCESS;
		}
		NTSTATUS CloseCall(PDRIVER_OBJECT pDriverObject, IRP irp)
		{
			irp.IoStatus.Status = STATUS_SUCCESS;
			irp.IoStatus.Information = 0;
			IofCompleteRequest(&irp, IO_NO_INCREMENT);
			return STATUS_SUCCESS;
		}
		NTSTATUS CloseDriver(PDRIVER_OBJECT pDriverObject)
		{
			DbgPrintEx(0, 0, "Ending Driver Session");
			IoDeleteSymbolicLink(&SymbosName);
			IoDeleteDevice(pDriverObject->DeviceObject);
		}
		NTSTATUS IoControl(PDRIVER_OBJECT pDriverObject, IRP irp)
		{
			
		}
		PVOID SuperHackerReading();
		NTSTATUS EndProcess(ULONG TargetPID)
		{
			NTSTATUS NtRet = STATUS_SUCCESS;
			PEPROCESS PeProc = { 0 };
			NtRet = PsLookupProcessByProcessId((HANDLE)TargetPID, &PeProc);
			if (NtRet != STATUS_SUCCESS)
				return NtRet;
			
			HANDLE ProcessHandle;
			NtRet =  ObOpenObjectByPointer(PeProc, 0, 0, 25, *PsProcessType, KernelMode, &ProcessHandle);
			if (NtRet != STATUS_SUCCESS)
				return NtRet;
			ZwTerminateProcess(ProcessHandle, 0);
			ZwClose(ProcessHandle);
			return NtRet;
		}
		VOID CLOSEAntiCheats() {
			
		}

			
	};
};