#include <windows.h>
#include <mutex>
#include <string>
#include <vector>
#include <TlHelp32.h>

#include "Overlay.hpp"

#pragma comment( lib, "ntdll.lib" )


int main()
{
	SetConsoleTitle(("Rust Private"));
	if (CallD.setup())
		std::cout << "[+] Function Setup Successful\n";

	if (!CallD.setup()) {
		driver.set_pid("RustClient.exe");
		if (driver.get_pid()) {
			exit(2);
		}
	}

	if (!driver.is_mapped()) { std::cout << "lol"; }
	else std::cout << ("[+] Driver Mapped | Loaded\n");

	std::cout << "Please Launch RustClient.exe\n";

	while (driver.get_pid() == NULL) {
		driver.set_pid("RustClient.exe");
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
		//Base process
	driver.get_module_base("RustClient.exe");
	RustClient = driver.get_base();
	driver.get_module_base("GameAssembly.dll");
	GameAssembly = driver.get_base();
	driver.get_module_base("UnityPlayer.dll");
	UnityPlayer = driver.get_base();

		// Print
	system("cls");
	printf("RustClient.exe | 0x%lx\n", RustClient);
	printf("GameAssembly.dll | 0x%lx\n", GameAssembly);
	printf("UnityPlayer.dll | 0x%lx\n", UnityPlayer);

	//begin overlay
	Overlay* Draw = new Overlay;
	Draw->DrawOverlay();
	delete Draw;
}