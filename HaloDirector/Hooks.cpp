#include "pch.h"
#include "StdInc.h"

#include "Renderer.h"

#include "kiero.h"
#include <d3d11.h>
#include <D3DX11.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
using namespace Halo;

#define DX11_PRESENT_INDEX 8

MSG msg;
DWORD current_process = 0;

Dx11Renderer	m_pDx11Renderer;

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

typedef HRESULT(__stdcall* D3D11Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
D3D11Present o_D11Present = NULL;


ID3D11Device* d3d11Device;
ID3D11DeviceContext* d3d11DevCon;
IDXGISwapChain* swapChain;
ID3D11Device* device;
ID3D11DeviceContext* context;
bool g_PresentHooked = false;

HRESULT GetDeviceAndCtxFromSwapchain(IDXGISwapChain* pSwapChain, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext)
{
	HRESULT ret = pSwapChain->GetDevice(__uuidof(ID3D11Device), (PVOID*)ppDevice);

	if (SUCCEEDED(ret))
		(*ppDevice)->GetImmediateContext(ppContext);

	return ret;
}

HRESULT __stdcall hkPresentD11(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {

	static bool init = false;
	if (!init)
	{
		g_PresentHooked = true;
		HRESULT devCtxHr = GetDeviceAndCtxFromSwapchain(pSwapChain, &device, &context);
		DXGI_SWAP_CHAIN_DESC sd;
		pSwapChain->GetDesc(&sd);

		d3d11Device = device;
		d3d11DevCon = context;

		Log::Info("Renderer initialized");
		Log::Info("[+] ID3D11DrawIndexed Addr: %p", (void*)pSwapChain);
		Log::Info("[+] ID3D11Device Addr: %p", (void*)device);


		init = true;
	}

	if (m_pDx11Renderer.IsRenderClassInitialized() == false)
	{
		m_pDx11Renderer.InitializeRenderClass(device, context);
	}
	else {
		m_pDx11Renderer.RenderText(10, 10, D3DCOLOR_ARGB(255, 0, 184, 245), "DirectX Draw Test");
		Log::Info("Drawing");
	}

	return o_D11Present(pSwapChain, SyncInterval, Flags);
}

void Hooks::Initialise()
{
	CreateThread(NULL, 0, &MouseHook, NULL, 0, NULL);
	current_process = GetCurrentProcessId();
	
	if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success && kiero::bind(DX11_PRESENT_INDEX, (void**)&o_D11Present, hkPresentD11) == kiero::Status::Success)
	{
		Log::Info("Hooked DirectX 11");
	}
	else {
		Log::Error("Unable to Hook DirectX 11");
	}
	
	

    Log::Info("Hooks Initialised");
} 
