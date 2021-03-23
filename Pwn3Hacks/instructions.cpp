#include "pch.h"

// This file contains the hooks that we want to overwrite

namespace {
	DWORD speedJmpBackAddy;
	DWORD heightRegister = 0;
	float walkSpeed;
	void __declspec(naked) speedHook() {
		__asm {
			//push eax
			//mov eax, walkSpeed
			//mov dword ptr[ecx+0x120], eax
			//pop eax
			//fld dword ptr[ecx+0x120] // We can re-arrange where this gets called based on esi


			fld walkSpeed

			jmp[speedJmpBackAddy] // Jump back to original code
		}
	}
}

