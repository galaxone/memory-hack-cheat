#include <iostream>
#include <windows.h>
#include <TlHelp32.h>

#include "keystrokes.cpp"
#include "memory.cpp"

int main() {

    std::cout << "AssaultCube Trainer" << std::endl;
    std::cout << "---------------------" << std::endl;
    std::cout << "PRESS KEYS: " << std::endl;
    std::cout << " NUMPAD1: Increase Health" << std::endl;
    std::cout << " NUMPAD2: Increase SMG Ammo" << std::endl;
    std::cout << " ESC: Exit" << std::endl;

    targetPID = GetProcessIdByName(processName);
    if (targetPID == 0) {
        std::cerr << "ERROR: Failed to get '"<<processName<<"' process ID." << std::endl;
        return 1;
    }

    moduleBaseAddress = GetModuleBaseAddress(targetPID, moduleName);
    std::cout << "Module Base Address: " << std::hex << moduleBaseAddress << std::endl;

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, targetPID);
    if (hProcess == NULL) {
        std::cerr << "ERROR: Failed to open '"<<processName<<"' process handle." << std::endl;
        return 1;
    }

    playerPtr = moduleBaseAddress + player;
    std::cout<<"Address to Read: "<<std::hex<<playerPtr<<std::endl;

    playerAddress = 0;
    if (!ReadProcessMemory(hProcess, reinterpret_cast<const void *>(playerPtr), &playerAddress, sizeof(playerAddress), NULL)) {
        std::cerr << "Failed to read from memory address." << std::endl;
        CloseHandle(hProcess);
        return 1;
    }
    std::cout<<"Player Address: "<<playerAddress<<std::endl;

    healthAddress = playerAddress + health;
    SMGMagAddress = playerAddress + SMGMag;
    std::cout<<"Health Address: "<<std::hex<<healthAddress<<std::endl;
    std::cout<<"SMG Mag Ammo Address: "<<std::hex<<SMGMagAddress<<std::endl;


    keystrokeThread = CreateThread(NULL, 0, KeyStrokeThread, NULL, 0, NULL);
    if (keystrokeThread == NULL) {
        std::cerr << "ERROR: Failed to create keystroke thread." << std::endl;
        CloseHandle(hProcess);
        return 1;
    }

    WaitForSingleObject(keystrokeThread, INFINITE);

    CloseHandle(hProcess);
    
    return 0;
}
