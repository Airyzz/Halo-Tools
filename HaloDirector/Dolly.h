#pragma once
#include "StdInc.h"

#define MAX_DOLLY_MARKERS 50

static class Dolly
{
public:
	static CameraMarker markers[MAX_DOLLY_MARKERS];
	static int count;
	static void play();
	static void update(int arg);
	static void addMarker();
	static void removeMarker(int index);
	static void removeAll();
	static void Initialise();
	static DWORD WINAPI Loop(LPVOID Param);
};

