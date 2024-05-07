#pragma once

#include <windows.h>
#include <cstdint>

HANDLE keystrokeThread;
HHOOK g_hook;

const char* processName = "ac_client.exe"; // Change this to the name of the target process
const wchar_t* moduleName = L"ac_client.exe"; // Change this to the name of the target module

DWORD targetPID;
HANDLE hProcess;

uintptr_t moduleBaseAddress;
uintptr_t playerPtr;

uintptr_t playerAddress = 0; // To store the player's address
uintptr_t healthAddress = 0; // To store the health address
uintptr_t SMGMagAddress = 0; // To store the SMG Mag Ammo address


// ----- Offsets -----
int player = 0x0017E0A8;
    int health = 0xEC; // Health Offset from 'player'
    int incHealth = 0x3E8; // Increment Health by 1000

    int SMGMag = 0x140; // SMG Loaded Mag Ammo Offset from 'player'
    int incAmmo = 0x64; // Increment Ammo by 100