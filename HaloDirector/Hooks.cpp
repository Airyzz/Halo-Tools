#include "pch.h"
#include "StdInc.h"
#include "UI.h"


using namespace Halo;

#define DX11_PRESENT_INDEX 8

MSG msg;
DWORD current_process = 0;

struct Patch {
	void* address;
	const char* bytes[50];
	int length;
};

int patchcount = 0;
Patch* patches[50];
bool Hooked = false;

DWORD64 Hooks::CreateHook(void* toHook, void* hk_func, int len)
{
	if (len < 14) {                                            //if less than 13 bytes
		return 0;                                         //we gtfo
	}

	//allocate 50 byte buffer
	Hooked = true;

	DWORD curProtection;
	VirtualProtect(toHook, len, PAGE_EXECUTE_READWRITE, &curProtection);

	//copy the original bytes, for restoring

	patches[patchcount] = new Patch();

	memcpy(patches[patchcount]->bytes, toHook, len);
	patches[patchcount]->length = len;
	patches[patchcount]->address = toHook;
	patchcount++;

	memset(toHook, 0x90, len);

	unsigned char patch[] = {
		0xFF, 0x25,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 //Address goes here
	};
	*(DWORD64*)&patch[6] = (DWORD64)hk_func; //replacing zeros with our function address

	memcpy((void*)toHook, patch, sizeof(patch));
	DWORD temp;
	VirtualProtect(toHook, len, curProtection, &temp);
	return((DWORD64)toHook) + len;
}


#pragma region MouseHook

bool get_state() {
	if (GetMessage(&msg, GetActiveWindow(), 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

bool is_foreground() {
	HWND foreground = GetForegroundWindow();
	DWORD foregroundID = 0;
	GetWindowThreadProcessId(foreground, &foregroundID);

	return (foregroundID == current_process);
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
				if (Cam)
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
							Cam->rotation.z += Math::radians(5);
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
							Cam->rotation.z -= Math::radians(5);
						}
					}
				}
			}
		}
		else if (wParam == WM_MBUTTONDOWN)
		{
			HWND foreground = GetForegroundWindow();
			DWORD foregroundID = 0;
			GetWindowThreadProcessId(foreground, &foregroundID);

			Log::Debug("Resetting to default");

			if (foregroundID == current_process)
			{
				if (GetKeyState(VK_SHIFT) & 0x8000)
				{
					*fov = 80.0;
				}
				else {
					Cam->rotation.z = 0;
				}
			}
		}
	}
	return CallNextHookEx(0, nCode, wParam, lParam);
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	BOOL fEatKeystroke = FALSE;

	if (nCode == HC_ACTION && is_foreground())
	{

		switch (wParam)
		{
		case WM_KEYUP:
		case WM_SYSKEYUP:
			PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
			switch (p->vkCode)
			{
				case VK_F2:
					Settings::draw_camera_path = !Settings::draw_camera_path;
					break;
				case 'Q':
					Log::Info("Executing: %s", UI::GetCurrentName());
					//Dolly::addMarker();
					UI::Do();
					break;
				case '1':
					UI::Left();
					Log::Info("Index: %d  ->  %s", UI::GetIndex(), UI::GetCurrentName());
					break;
				case '2':
					UI::Right();
					Log::Info("Index: %d  ->  %s", UI::GetIndex(), UI::GetCurrentName());
					break;
				case VK_INSERT:
					Dolly::addMarker();
					Log::Info("Inserting Camera Marker");
					break;
				case VK_NEXT:
					if (*Halo::timescale - 0.1 >= 0)
						*Halo::timescale -= 0.1;
					else 
						*Halo::timescale = 0.0;

					break;
				case VK_PRIOR:
					if (*Halo::timescale < 10.0f)
						*Halo::timescale += 0.1;
					break;
			}
			break;
		}
	}
	return(fEatKeystroke ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam));
}

DWORD WINAPI MouseHook(LPVOID param)
{
	HHOOK mousehook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, NULL, 0);
	HHOOK hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);

	while (true)
	{
		get_state();
		Sleep(20);
		UnhookWindowsHookEx(mousehook);
		return 0;
	}
}

#pragma endregion

#pragma region Keyboard Hook

DWORD WINAPI KeyboardHook(LPVOID param) {
	

	//while (true) {
	//
	//}

}

#pragma endregion

#pragma region CameraHook

DWORD64 CameraHook_Return;
void __declspec(naked) Camera_Hook() {

	__asm {
		movss [rsi+0x24],xmm1
		mulss xmm0,xmm2
		addss xmm0,[rsi+0x28]
		mov Cam,rsi
		jmp[CameraHook_Return]
	}

}

#pragma endregion

void Hooks::Initialise()
{
	CreateThread(NULL, 0, &MouseHook, NULL, 0, NULL);
	//CreateThread(NULL, 0, &KeyboardHook, NULL, 0, NULL);
	current_process = GetCurrentProcessId();

	CameraHook_Return = CreateHook((void*)Halo::CameraHookAddress, &Camera_Hook, 14);

    Log::Info("Hooks Initialised");
} 
