#pragma once

#include <iostream>
#include <Windows.h>

#include "constants.h"

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        if (wParam == WM_KEYDOWN) {
            KBDLLHOOKSTRUCT* pkbhs = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

            if(pkbhs->vkCode == VK_NUMPAD1){
                int healthValue = 0;
                if (!ReadProcessMemory(hProcess, reinterpret_cast<const void *>(healthAddress), &healthValue, sizeof(healthValue), NULL)) {
                    std::cerr << "Failed to read from memory address." << std::endl;
                    CloseHandle(hProcess);
                    return 1;
                }
                std::cout << "Health Value: " <<std::dec<< healthValue << std::endl;

                healthValue = healthValue + incHealth;
                std::cout<<"New Health Value: "<<std::dec<<healthValue<<std::endl;

                if (!WriteProcessMemory(hProcess, reinterpret_cast<void *>(healthAddress), &healthValue, sizeof(healthValue), NULL)) {
                    std::cerr << "Failed to write to memory address." << std::endl;
                    CloseHandle(hProcess);
                    return 1;
                }

                std::cout << "Health Value successfully changed to: " << std::dec << healthValue << std::endl;
            }

            if(pkbhs->vkCode == VK_NUMPAD2){
                int ammoValue = 0;
                if(!ReadProcessMemory(hProcess, reinterpret_cast<const void*>(SMGMagAddress), &ammoValue, sizeof(ammoValue), NULL)){
                    std::cerr << "ERROR: Failed to read from memory address." << std::endl;
                    CloseHandle(hProcess);
                    return 1;
                }
                std::cout << "SMG Mag Ammo Value: " << std::dec << ammoValue << std::endl;
                ammoValue = ammoValue + incAmmo;
                std::cout << "New SMG Mag Ammo Value: " << std::dec << ammoValue << std::endl;

                if(!WriteProcessMemory(hProcess, reinterpret_cast<void *>(SMGMagAddress), &ammoValue, sizeof(ammoValue), NULL)){
                    std::cerr << "ERROR: Failed to write to memory address." << std::endl;
                    CloseHandle(hProcess);
                    return 1;
                }
            }

            if(pkbhs->vkCode == VK_ESCAPE){
                PostQuitMessage(0);
            }
        }

    }

    // Pass the event to the next hook procedure in the hook chain
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}


DWORD WINAPI KeyStrokeThread(LPVOID lpParam) {
    g_hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (g_hook == NULL) {
        return 0;
    }

    // Keep the program running to continue monitoring keystrokes
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Unhook the keyboard hook when the program exits
    UnhookWindowsHookEx(g_hook);
    return 1;
}