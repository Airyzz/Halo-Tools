#pragma once

#include <iostream>
#include <Windows.h>

#include "hD3D9.h"

#include <d3dx9.h>
#include <d3d9.h>
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

extern int windowWidth; // Spielfenster Breite
extern int windowHeight; // Spielfenster Höhe
extern char lWindowName[256]; // Overlay Name
extern HWND hWnd; // Overlay Handle
extern char tWindowName[256]; // Ziel Name
extern HWND TargetWnd; // Ziel Handle
extern RECT WindowRect, ClientRect; // Overlay, Spiel RECT
extern MSG Message; // Queue Nachricht

static class Drawing
{
public:
	static void Draw();
	static void Init();
};

