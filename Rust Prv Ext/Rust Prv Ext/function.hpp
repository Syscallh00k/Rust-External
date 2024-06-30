#pragma once

#include "Includes.hpp"


enum control
{
    read = 444,
    write = 555,
    base = 666,
};

struct cmd_t
{
public:
    /* driver instructions */
    DWORD key;
    control code;

    /* parameters */
    DWORD process_id;

    /* module base */
    PVOID outbase;
    wchar_t* module_name;

    /* read/write */
    PVOID address;
    PVOID value;
    size_t size;
};

class function_t
{
private:
    int64_t(__fastcall* call_driver)(PVOID a1, PVOID a2, PVOID a3, PVOID a4, PVOID a5, PVOID a6);
public:
    bool setup()
    {
        HMODULE user32 = LoadLibraryA("user32.dll");
        HMODULE win32u = LoadLibraryA("win32u.dll");

        *(PVOID*)&call_driver = GetProcAddress(win32u, ("NtUserCreateDesktopEx"));

        return call_driver;
    }

    void call(cmd_t* cmd)
    {
        call_driver(cmd, 0, 0, 0, 0, 0);
    }
}CallD;