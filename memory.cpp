#pragma once

#include <iostream>
#include <windows.h>
#include <TlHelp32.h>


DWORD GetProcessIdByName(const char* processName) {
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return 0;
    }

    if (!Process32First(hProcessSnap, &pe32)) {
        CloseHandle(hProcessSnap);
        return 0;
    }

    do {
        if (strcmp(pe32.szExeFile, processName) == 0) {
            CloseHandle(hProcessSnap);
            return pe32.th32ProcessID;
        }
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return 0;
}

uintptr_t GetModuleBaseAddress(DWORD processId, const wchar_t* moduleName) {
    uintptr_t moduleBaseAddress = 0;

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | 0x00000008, processId);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        MODULEENTRY32W moduleEntry; // Note: Use MODULEENTRY32W for wide character version
        moduleEntry.dwSize = sizeof(moduleEntry);

        if (Module32FirstW(hSnapshot, &moduleEntry)) { // Note: Use Module32FirstW
            do {
                if (wcscmp(moduleEntry.szModule, moduleName) == 0) {
                    moduleBaseAddress = reinterpret_cast<uintptr_t>(moduleEntry.modBaseAddr);
                    break;
                }
            } while (Module32NextW(hSnapshot, &moduleEntry)); // Note: Use Module32NextW
        }

        CloseHandle(hSnapshot);
    }

    return moduleBaseAddress;
}