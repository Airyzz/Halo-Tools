#pragma once
#include "StdInc.h"

namespace Halo
{
	extern float *serverTime;

	extern float* serverSeconds;

	extern float* fov;

	extern DWORD64 CameraHookAddress;

	extern Camera* Cam;

	extern HWND pHwnd;

	extern float* viewMatrix;

	extern HMODULE hMod;

	extern float* timescale;

	DWORD64 Scan(LPCWSTR modName, const char* pattern, const char* mask);

	void Initialise();

	char* ScanIn(const char* pattern, const char* mask, char* begin, unsigned int size);
};

