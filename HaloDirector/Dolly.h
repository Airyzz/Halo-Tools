#pragma once
#include "StdInc.h"

#define MAX_DOLLY_MARKERS 50

static class Dolly
{
public:
	static CameraMarker markers[MAX_DOLLY_MARKERS];
	static int count;
	static void play();
	static void ToggleDolly();
	static bool IsDollying();
	static void update(int arg);
	static void addMarker();
	static void removeMarker(int index);
	static void removeSelected();
	static void removeAll();
	static void Initialise();
	static bool DollyExistsAtCurrentTime();
	static int GetNextMarkerInTimeline();
	static void SkipToNextMarker();
	static void SkipToMarker(int index);
	static DWORD WINAPI Loop(LPVOID Param);
	static Vector3 GetPositionForCurrentTime();
	static bool BetweenMarkers();
	static int GetSelectedMarkerIndex();
	static void EditMarker();
};

