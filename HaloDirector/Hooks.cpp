#include "pch.h"
#include "StdInc.h"
using namespace Halo;
MSG msg;
DWORD current_process = 0;
bool get_state() {
	if (GetMessage(&msg, GetActiveWindow(), 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= 0) {
		if (wParam == WM_MOUSEWHEEL)
		{
			MSLLHOOKSTRUCT* pMhs = (MSLLHOOKSTRUCT*)lParam;
			short zDelta = HIWORD(pMhs->mouseData);
			HWND foreground = GetForegroundWindow();
			DWORD foregroundID = 0;
			GetWindowThreadProcessId(foreground, &foregroundID);


			if (foregroundID == current_process)
			{
				if (zDelta < 0)
				{
					//Down

					if (GetKeyState(VK_SHIFT) & 0x8000)
					{
						if (*fov + 5.0f < 150.0f)
						{
							*fov += 5.0f;
						}
						else {
							*fov = 150.0f;
						}
					}
					else {
						(*world)->player->camera.rotation.z += Math::radians(5);
					}
					
				}
				else {
					//Up
					if (GetKeyState(VK_SHIFT) & 0x8000)
					{
						if (*fov - 5.0f > 1.0f) {
							*fov -= 5.0f;
						}
						else {
							*fov = 1.0f;
						}
					}
					else {
						(*world)->player->camera.rotation.z -= Math::radians(5);
					}
				

				}
			}
		}
	}
	return CallNextHookEx(0, nCode, wParam, lParam);
}

DWORD WINAPI MouseHook(LPVOID param)
{
	HHOOK mousehook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, NULL, 0);
	while (true)
	{
		get_state();
		Sleep(20);
		UnhookWindowsHookEx(mousehook);
		return 0;
	}
}

DWORD64 Hooks::CreateHook(void* toHook, void* hk_func, int len)
{
	return DWORD64();
}

void Hooks::Initialise()
{
	CreateThread(NULL, 0, &MouseHook, NULL, 0, NULL);
	current_process = GetCurrentProcessId();
    Log::Info("Hooks Initialised");
}
