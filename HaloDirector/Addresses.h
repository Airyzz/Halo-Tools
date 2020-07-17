#pragma once
#include "StdInc.h"

namespace Halo
{
	extern float *serverTime;
	extern float* serverSeconds;
	extern float* fov;
	extern World** world;

	DWORD64 Scan(LPCWSTR modName, const char* pattern, const char* mask);
	void Initialise();
	char* ScanIn(const char* pattern, const char* mask, char* begin, unsigned int size);
};

