// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "stdafx.h"
#include <iostream>
#include "mem.h"
#include "proc.cpp"
#include <concepts>
#include "instructions.cpp"
#include "originalFuncs.cpp"
#include "dllmain.h"

// Modified from https://guidedhacking.com/threads/how-to-hack-any-game-tutorial-c-trainer-1-external.10897/

// Injected via https://github.com/multikill/GH_Injector_MSVC_2019_QT_5_15_0/releases

// try with __stdcall too  // __cdecl, __thiscall
typedef void (__thiscall* _MyFunc)(const char * text);
_MyFunc myFunc;

void teleport(uintptr_t procBase, float x, float y, float z) {
    *(float*)mem::FindDMAAddy(procBase + 0x018FFDE4, { 0x4, 0x4, 0x1D4, 0x408, 0x24C, 0x180, 0x90 }) = x;
    *(float*)mem::FindDMAAddy(procBase + 0x018FFDE4, { 0x4, 0x4, 0x1D4, 0x408, 0x24C, 0x180, 0x94 }) = y;
    *(float*)mem::FindDMAAddy(procBase + 0x018FFDE4, { 0x4, 0x4, 0x1D4, 0x408, 0x24C, 0x180, 0x98 }) = z;
}

void showEnemiesLoc(uintptr_t base) {
    for (unsigned int i = 0x0; i < 0x1A; i += 0xC) {
        for (unsigned int j = 0x0; j <= 0x8; j += 0x4) {
            std::cout << *(float*)mem::FindDMAAddy(base + 0x0166C150, { 0xF7C, 0x378 + i, 0xD4, 0x90 + j });
            std::cout << " , ";
        }
        std::cout << "\n";
    }
}

DWORD WINAPI HackThread(HMODULE hModule) {
    //Create Console
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    std::cout << "Starting PwnAdventure3 Hack\n";
    std::cout << "Key binds:\n";
    std::cout << "  [End] to quit\n";
    std::cout << "  [Numpad 1] for Test Script              [Toggle]\n";
    std::cout << "  [Numpad 2] for Fly Hack                 [Toggle]\n";
    std::cout << "  [Numpad 3] for Infinite Ammo            [Toggle]\n";
    std::cout << "  [Numpad 4] for Teleport to Fire Spell\n";
    std::cout << "  [Numpad 5] for Teleport to Gun Shop\n";
    std::cout << "  [Numpad 6] for Teleport to Rats\n";
    std::cout << "  [Numpad 7] for Speed Hack\n\n";

    DWORD procId = GetProdId(L"PwnAdventure3-Win32-Shipping.exe");

    uintptr_t procBase = (uintptr_t)GetModuleHandle(L"PwnAdventure3-Win32-Shipping.exe");
    uintptr_t moduleBase = GetModuleBaseAddress(procId, L"GameLogic.dll");

    bool bHealth = false, bAmmo = false, bHeight = false, bSpeed = false;
    
    DWORD zAxisPtr;
    float zAxisValue;

    // Define original game functions here
    //typedef float*(__stdcall* _oRespawn)();
    //_oRespawn oRespawn = (_oRespawn)(0x7A97FF70);

    // moduleBase + 0x4FF70 - GetManna
    myFunc = (_MyFunc)(moduleBase + 0x551A0);

    //Create an instance of the function and assign it to an address

    

    /*
    Coordinates:

        Spell Book:
        x - -43644.0
        y - -56041.0
        z - 308.0


        Shop:
        x - -37496
        y - -18540
        z - 2514

        Rats:
        x - -53490
        y - -41045
        z - 782
    
    */

    while (true) {
        if (GetAsyncKeyState(VK_DELETE) & 1) 
            break;

        /*if (GetAsyncKeyState(VK_DOWN) & 1) {
            std::cout << "Test function\n";
        }*/

        if (GetAsyncKeyState(VK_NUMPAD1) & 1) {
            bHealth = !bHealth;
            //oRespawn();
            char text[10] = "hey";
            myFunc(text);
            std::cout <<" Done the thing\n";
            //showEnemiesLoc(procBase);
        }

        if (GetAsyncKeyState(VK_NUMPAD2) & 1) {
            bHeight = !bHeight;
            if (bHeight) {
                std::cout << "[+] Enabling fly hack\n";
                zAxisPtr = mem::FindDMAAddy(procBase + 0x018FFDE4, { 0x4, 0x4, 0x1D4, 0x408, 0x24C, 0x180, 0x98 });
                zAxisValue = *(float*)zAxisPtr;
            }
            else {
                std::cout << "[-] Disabling fly hack\n";
            }

        }

        //Infinite ammo NOP
        if (GetAsyncKeyState(VK_NUMPAD3) & 1) {
            bAmmo = !bAmmo;

            if (bAmmo) {
                std::cout << "[+] Enabling infinite ammo\n";
                mem::Nop((BYTE*)(moduleBase + 0x52396), 3);
            }
            else {
                std::cout << "[-] Disabling infinite ammo\n";
                //89 48 1C the original stack setup and call
                mem::Patch((BYTE*)(moduleBase + 0x52396), (BYTE*)"\x89\x48\x1c", 3);
            }
        }

        if (GetAsyncKeyState(VK_NUMPAD4) & 1) {
            // Teleport to the Fire Spell
            std::cout << "[+] Teleporting to Fire Spell Book\n";
            teleport(procBase, -43644.0f, -56041.0f, 309.0f);
        }

        if (GetAsyncKeyState(VK_NUMPAD5) & 1) {
            // Teleport to the Gun Shop
            std::cout << "[+] Teleporting to Gun Shop\n";
            teleport(procBase, -37496.0f, -18540.0f, 2514.0f);
        }

        if (GetAsyncKeyState(VK_NUMPAD6) & 1) {
            // Teleport to the Rats
            std::cout << "[+] Teleporting to Rats\n";
            teleport(procBase, -53490.0f, -41045.0f, 782.0f);
        }

        if (GetAsyncKeyState(VK_NUMPAD7) & 1) {
            bSpeed = !bSpeed;
            DWORD speedHookAddress = moduleBase + 0x4FF90;
            int speedHookLength = 6;
            if (bSpeed) {
                speedJmpBackAddy = speedHookAddress + speedHookLength;
                std::cout << "[-] Hooking into walking speed method\n";
                walkSpeed = 2000.0f;
                if (mem::Hook((void*)speedHookAddress, speedHook, speedHookLength)) {
                    std::cout << "[+] Enabled speed hack\n";
                }
            }
            else {
                std::cout << "[-] Disabling speed hack\n";
                // Bytes D9 81 20 01 00 00
                mem::Patch((BYTE*)(speedHookAddress), (BYTE*)"\xD9\x81\x20\x01\x00\x00", speedHookLength);
                *(float*)mem::FindDMAAddy(procBase + 0x01900600, { 0xAC, 0x3C, 0x438, 0x18, 0x3E0, 0x120 }) = 200; // Default speed

            }

            // 0F29 86 90000000 Original code
            //if (bHeight) {
            //    hookLength = 7;
            //    DWORD hookAddress = blah;
            //    jmpBackAddy = hookAddress + hookLength;
            //    std::cout << "[-] Hooking into height method\n";
            //    if (mem::Hook((void*)hookAddress, heightHook, hookLength)) {
            //        std::cout << "[+] Hooked into height method!\n";
            //        float* value = (float *) heightRegister + 0x90;
            //        std::cout  << " " << value << " register value \n";
            //    }
            //}
            //else {
            //    std::cout << "[-] Disabling infinite ammo\n";
            //    //89 48 1C the original stack setup and call
            //    mem::Patch((BYTE*)(hookAddress), (BYTE*)"\x0F\x29\x86\x90\x00\x00\x00", hookLength);
            //}
        }


        //need to use uintptr_t for pointer arithmetic later
        uintptr_t* localPlayerPtr = (uintptr_t*)(moduleBase + 0x10F4F4);
        bool canRun = true; // Used this because I haven't got a value to check yet to make sure it doesnt fail

        //continuous writes / freeze

        if (canRun) {

            if (bHeight) {
                if (GetAsyncKeyState(VK_SPACE)) {
                    zAxisValue += 8.0f;
                }
                
                if (GetAsyncKeyState(VK_CONTROL)) {
                    zAxisValue -= 8.0f;
                }

                *(float*)zAxisPtr = zAxisValue;
                
            }
            //if (bHealth) {
            //    *(int*)(*localPlayerPtr + 0xF8) = 1337;
            //}

            //if (bAmmo)
            //{
            //    //We aren't external now, we can now efficiently calculate all pointers dynamically
            //    //before we only resolved pointers when needed for efficiency reasons
            //    //we are executing internally, we can calculate everything when needed
            //    uintptr_t ammoAddr = mem::FindDMAAddy(moduleBase + 0x10F4F4, { 0x374, 0x14, 0x0 });
            //    int* ammo = (int*)ammoAddr;
            //    *ammo = 1337;

            //    //or just
            //    *(int*)mem::FindDMAAddy(moduleBase + 0x10F4F4, { 0x374, 0x14, 0x0 }) = 1337;
            //}

        }
        Sleep(5);
    }

    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH: 
        {
            HANDLE thread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr);
            if (thread)
                CloseHandle(thread);
        }
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}