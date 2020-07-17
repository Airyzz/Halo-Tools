#pragma once
#include "StdInc.h"

struct RegisterStore {

	DWORD64 rax;
	DWORD64 rcx;
	DWORD64 rdx;
	DWORD64 rbx;
	DWORD64 rbp;
	DWORD64 rsi;
	DWORD64 rdi;
	DWORD64 rsp;
	DWORD64 rip;
	DWORD64 r8;
	DWORD64 r9;
	DWORD64 r10;
	DWORD64 r11;
	DWORD64 r12;
	DWORD64 r13;
	DWORD64 r14;
	DWORD64 r15;
};

class Hooks
{
public:
	static DWORD64 CreateHook(void* toHook, void* hk_func, int len);

	static void Initialise();
};

