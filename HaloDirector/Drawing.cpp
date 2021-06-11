#include "Drawing.h"
#include <Windows.h>
#include <TlHelp32.h>
#include "StdInc.h"


HWND hWnd, TargetWnd;
MSG Message;
RECT WindowRect, ClientRect;
int windowWidth, windowHeight;
int clientWidth = 1600, clientHeight = 900;
int borderWidth, borderHeight; // Grenz Größen


char lWindowName[256] = "Overlay";
char tWindowName[256] = "Halo: The Master Chief Collection  "; // put Game window name here

const MARGINS pMargin = { 0,0, clientWidth, clientHeight };


LRESULT CALLBACK Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{

	case WM_PAINT: // we need to paint? lets paint!
		Render();
		break;
	case WM_CREATE:
		return DwmExtendFrameIntoClientArea(hWnd, &pMargin); // extension of window frame into client area
		break;
	case WM_DESTROY:
		PostQuitMessage(0); // We need to use this to exit a message loop
		break;
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam); // Making sure all messages are processed
		break;
	}
}

void Drawing::Draw()
{
	
}

void Drawing::Init()
{
	WNDCLASSEXA OverlayWnd; // contains window class information
	OverlayWnd.cbSize = sizeof(WNDCLASSEXA); // size of struct, basically checking for version or check
	OverlayWnd.style = CS_HREDRAW | CS_VREDRAW;  // Style, redraw method type
	OverlayWnd.lpfnWndProc = Proc; // Pointer to the window procedure
	OverlayWnd.cbClsExtra = 0; // window class struct extra bytes
	OverlayWnd.cbWndExtra = 0; // window instance extra bytes
	OverlayWnd.hInstance = Halo::hMod; // handle to the instance that contains the window procedure for the class
	OverlayWnd.hIcon = LoadIcon(NULL, IDI_APPLICATION); // basic window icon set
	OverlayWnd.hIconSm = LoadIcon(NULL, IDI_APPLICATION); // basic window icon set
	OverlayWnd.hCursor = LoadCursor(NULL, IDC_ARROW); // basic window cursor icon set
	OverlayWnd.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0)); // handle to the class background brush
	OverlayWnd.lpszMenuName = lWindowName;
	OverlayWnd.lpszClassName = lWindowName;

	// registers a window class for the use in call to this createwindowex func
	if (!RegisterClassExA(&OverlayWnd))
	{
		exit(1);
	}

	TargetWnd = FindWindowA(0, tWindowName);



	if (TargetWnd)
	{
		GetWindowRect(TargetWnd, &WindowRect);
		windowWidth = WindowRect.right - WindowRect.left;
		windowHeight = WindowRect.bottom - WindowRect.top;
		hWnd = CreateWindowExA(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, lWindowName, lWindowName, WS_POPUP, 1, 1, windowWidth, windowHeight, 0, 0, 0, 0);
		Log::Info("Succesfully got hWnd!");
	}
	else {
		Log::Error("Unable to find hWnd");
	}


	/*
	SetLayeredWindowAttributes sets the opacity and transparency color key for a layered window.
	*/
	SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
	SetLayeredWindowAttributes(hWnd, 0, 255, LWA_ALPHA);

	/*
	Show the layered window aka our overlay.
	*/
	ShowWindow(hWnd, SW_SHOW);

	/*
	We use our handle to our overlay and initalize our D3D adapter.
	*/
	D3D9Init(hWnd);

	bool panic = false;

	/*
	While we are not panicking, we will be enable our hack.
	*/
	while (panic == false)
	{

		if (GetAsyncKeyState(VK_F12))
			panic = true;

		/*
		Dispatches incoming sent messages, checks the thread message queue for a posted message, and retrieves the message (if any exist). Messages are removed from the queue after processing due to PM_REMOVE.
		*/
		if (PeekMessage(&Message, hWnd, 0, 0, PM_REMOVE))
		{
			/*
			Translates virtual-key messages into character messages.
			*/
			TranslateMessage(&Message);

			/*
			Dispatches a message to a window procedure.
			*/
			DispatchMessage(&Message);
		}

		/*
		Set the hWnd for the game we want.
		*/
		TargetWnd = FindWindowA(0, tWindowName);

		/*
		If there is no game, quit.
		*/
		if (!TargetWnd)
			exit(0);

		/*
		Set the RECT using the targeted window.
		*/
		GetWindowRect(TargetWnd, &WindowRect);
		GetClientRect(TargetWnd, &ClientRect);

		windowWidth = WindowRect.right - WindowRect.left;
		windowHeight = WindowRect.bottom - WindowRect.top;

		/*
		Resize the overlay matching what is required.
		*/
		DWORD dwStyle = GetWindowLong(TargetWnd, GWL_STYLE);

		if (dwStyle & WS_BORDER)
		{
			windowHeight = WindowRect.bottom - WindowRect.top;
			windowWidth = WindowRect.right - WindowRect.left;
			clientHeight = ClientRect.bottom - ClientRect.top;
			clientWidth = ClientRect.right - ClientRect.left;
			borderHeight = (windowHeight - ClientRect.bottom);
			borderWidth = (windowWidth - ClientRect.right) / 2; //only want one side
			borderHeight -= borderWidth; //remove bottom from width, bottom is the same size as either side so we subtract that

			WindowRect.left += borderWidth;
			WindowRect.top += borderHeight;
		}

		MoveWindow(hWnd, WindowRect.left, WindowRect.top, clientWidth, clientHeight, true);

	} 
}
