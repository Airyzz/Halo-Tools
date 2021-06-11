#include "StdInc.h"
#include "hD3D9.h"
#include <math.h>
#include "UI.h"

IDirect3D9Ex* dx_Object = NULL;
IDirect3DDevice9Ex* dx_Device = NULL;
D3DPRESENT_PARAMETERS dx_Params;
ID3DXLine* dx_Line;
ID3DXFont* dx_Font = 0;
int fontHeight;

/*
We require to initialize the D3D drawing, so we require hWnd. Windows identifies each form or application by assigning it a handle or also known as hWnd.
*/
int D3D9Init(HWND hWnd)
{
	 // We get our Process Access and Module Bases


	/*
	We need to check to see if we can create an IDirect3D9Ex object and return an interface to it. Why is D3D_SDK_VERSION passed? Because we will need to ensure that the header files used in the compiled application match the version of the installed runtime DLLs. Why are we passing the object to dx_Object? Because we are creating an IDirect3D9Ex object, and we need to store it somewhere. If it fails, the app crashes (the DLL), and if it passes, it continues, simple huh?
	*/
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &dx_Object)))
	{
		Log::Error("D3D Failed to Initiate");
		exit(1);
	}

	/*
	We created the dx_Param earlier, it is a D3DPRESENT_PARAMETERS structure. It contains many variables you can modify but in this source we are only modifying these variables.

	BackBufferFormat (D3DFORMAT) is the buffer that is drawn off-screen and will be switched with the front buffer at the next frame. This is considered double buffering, which is what you need to do in GDI to ensure that it does not flicker. But GDI will still flicker because it is "slow" you could persay.

	D3DFMT_A8R8G8B8	(Value: 21) is an 32-bit ARGB pixel format with alpha, using 8 bits per channel.
	*/

	dx_Params.BackBufferFormat = D3DFMT_A8R8G8B8;

	dx_Params.BackBufferWidth = windowWidth;
	dx_Params.BackBufferHeight = windowHeight;
	dx_Params.EnableAutoDepthStencil = TRUE;
	dx_Params.AutoDepthStencilFormat = D3DFMT_D16;

	/*
	hDeviceWindow (HWND) is the form or application that determines the location and size of the back buffer on the screen.
	*/
	dx_Params.hDeviceWindow = hWnd;

	/*
	MultiSampleQuality (DWORD) is the quality level. Technically speaking DEFAULT_QUALITY is zero which also is kind of funny because zero is the lowest MultiSampleQuality. Why are we setting this? Well this is all GPU related, and microsoft is extremely vauge about this, so we will just leave this as zero.
	*/
	dx_Params.MultiSampleQuality = DEFAULT_QUALITY;

	/*
	SwapEffect (D3DSWAPEFFECT) is how the front and back buffer are to be swapped. When we disregard this, we can do multi sampling (above).
	*/
	dx_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;

	/*
	Windowed (BOOL) is basically asking you if the form or application is running windowed or fullscreen. True is windowed. False is fullscreen.
	*/
	dx_Params.Windowed = TRUE;


	/*
	We need to see if we can create a device to REPRESENT the display adapter.
	D3DADAPTER_DEFAULT (UNIT) is the always the primary display adapter.
	D3DDEVTYPE_HAL (D3DDEVTYPE) (value: 1) is hardware rasterization. Shading is done with software, hardware, or mixed transform and lighting.
	hWnd (HWND) is the form we will create the device in mind with. Something I noticed was that this can be null if we set the hDeviceWindow to a non-null value, which it is. So I changed it to a null.
	D3DCREATE_HARDWARE_VERTEXPROCESSING (DWORD) specifies hardware vertex processing.
	dx_Param (D3DPRESENT_PARAMTERS*) describe the presentation parameters for the device to be created.
	0 (D3DDISPLAYMODEEX*) is only used when the device is set to fullscreen, which it is not for this source, but it basically is the properties of a display mode (size, width, height, refresh rate, format, scanlineordering).
	dx_Device (IDirect3DDevice9Ex**) is the address of a pointer to the returned IDirect3DDevice9Ex, which represents the created device.
	*/
	if (FAILED(dx_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, NULL, D3DCREATE_HARDWARE_VERTEXPROCESSING, &dx_Params, 0, &dx_Device)))
	{
		Log::Error("D3D Failed to Initiate");
		exit(1);
		
	}


	if (!dx_Line)
		D3DXCreateLine(dx_Device, &dx_Line);

	/*
	D3DXCreateFont creates a font object for a device and font.

	D3DXCreateFont(device, h, w, weight, miplevels, italic, charset, OutputPrecision, quality, pitchandfamily, pfaceanme, *ppfont)

	dx_Device (LPDIRECT3DDEVICE9) is the device we will be creating a font for.
	24 (INT) is the height of the characters in logical units.
	0 (UINT) is the width of the characters in logical units.
	FW_REGULAR (UNIT) is the typeface weight.
	0 (MipLevels) is the number of mipmap levels. MipMaps (not miplevels) are pre-calculated, optimized collections of images that accompany a main texture, intended to increase rendering speed and reduce aliasing artifacts
	false (BOOL) is if the italic font is true or not. In this case it is false.
	DEFAULT_CHARSET ( DWORD) is the character set of the font.
	OUT_CHARACTER_PRECIS (DWORD) specifies how Windows should attempt to match the desired font sizes and characteristics with actual fonts. In this case we are not using this feature.
	ANTIALIASED_QUALITY (DWORD) specifies how Windows should match the desired font with a real font. In this case we are always antialiasing if the font supports it and the size of the font is not too small or too large.
	DEFAULT_PITCH (DWORD) is the pitch and family index.
	Verdana (LPCTSTR) is the string containing the typeface name (font style).
	dx_Font (LPD3DXFONT*) returns a pointer to an ID3DXFont interface, representing the created font object.
	*/

	fontHeight = windowHeight / 40;

	D3DXCreateFont(dx_Device, fontHeight, 0, FW_LIGHT, 1, false, DEFAULT_CHARSET, OUT_DEVICE_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, L"Arial", &dx_Font);
	Log::Debug("D3D Initiated");
	return 0;
}


void DrawCameraMarkers() {

	float width = windowHeight / 10.0f;
	float hWidth = width / 2.0f;

	for (int i = 0; i < Dolly::count; i++)
	{
		CameraMarker* m0;
		CameraMarker* m1;
		CameraMarker* m2;
		CameraMarker* m3;


		if (i < MAX_DOLLY_MARKERS - 1)
		{
			if (i > 0)
			{
				m0 = &Dolly::markers[i - 1];
				m1 = &Dolly::markers[i];
			}
			else {
				m0 = &Dolly::markers[i];
				m1 = &Dolly::markers[i];
			}

			if ((Dolly::count - i) > 2)
			{
				m2 = &Dolly::markers[i + 1];
				m3 = &Dolly::markers[i + 2];
			}
			else {
				m2 = &Dolly::markers[i + 1];
				m3 = &Dolly::markers[i + 1];
			}

			//Calculate inbetween points and draw lines

			Vector3 last_pos;
			for (float alpha = 0; alpha < 1.01; alpha += 0.05) {

				Vector3 pos = Math::CatmullRomInterpolate(
					m0->position,
					m1->position,
					m2->position,
					m3->position,
					alpha,
					0.0f);

				Vector3 screenpos = Math::WorldToScreen(pos, windowWidth, windowHeight);
				//screenpos = Math::FixAspectRatio(screenpos, windowWidth, windowHeight);

				//if (screenpos.x != 0 && screenpos.y != 0 && last_pos.x != 0 && last_pos.y != 0)
				//{
				//	DrawLine(screenpos.x * (float)windowWidth, screenpos.y * (float)windowHeight, last_pos.x * (float)windowWidth, last_pos.y * (float)windowHeight, 0xFF, 0xFf, 0xFF, 100, 5.0f);
				//}
				DrawFilled(screenpos.x, screenpos.y, 10, 10, 255, 255, 255, 255/2);

				//last_pos = screenpos;
			}
		}

		CameraMarker* marker = &Dolly::markers[i];
		Vector3 screenpos = Math::WorldToScreen(marker->position, windowWidth, windowHeight);
		//screenpos = Math::FixAspectRatio(screenpos, windowWidth, windowHeight);

		float s_width = (width / (screenpos.z * 0.5));
		float sh_width = s_width / 2.0f;
		float outline_width = s_width + (sh_width / 2.0f);
		float outline_width_h = outline_width / 2.0f;

		//If near a marker, draw a highlight
		if (screenpos.z < 0.5)
			DrawFilled(screenpos.x - outline_width_h, screenpos.y - outline_width_h, outline_width, outline_width, 255, 255, 255, 255);

		//Calculate Time:

		int n = (int)round(marker->time);

		int day = n / (24 * 3600);

		n = n % (24 * 3600);
		int hour = n / 3600;

		n %= 3600;
		int minutes = n / 60;

		n %= 60;
		int seconds = n;

		//Draw Info
		std::string text = std::to_string(minutes) + ":" + std::to_string(seconds);

		
		int textW = GetTextWidth((char*)text.c_str(), dx_Font);
		DrawShadowString((char*)text.c_str(), screenpos.x - (((float)textW) / 2.0f), screenpos.y + (sh_width * 1.1), 0xFF, 0xFF, 0xFF, dx_Font);
		
		//Draw Camera Number
		//DrawShadowString((char*)std::to_string(i + 1).c_str(), ((screenpos.x) * (float)windowWidth), (screenpos.y * (float)windowHeight) - (sh_width * 1.3), 0xFF, 0xFF, 0xFF, dx_Font);

		//Draw Dolly Markers

		DrawFilled(screenpos.x  - sh_width, screenpos.y - sh_width, s_width, s_width, 0, 255, 255, 255);

		//Draw line to indicate directoin

		Vector3 forward_screen_pos = Math::WorldToScreen(marker->forward, windowWidth, windowHeight);

		DrawLine(screenpos.x, screenpos.y, forward_screen_pos.x, forward_screen_pos.y, 255, 255, 255, 255 / 2, 3);

		//Draw current dolly position
		if (Dolly::BetweenMarkers())
		{
			screenpos = Math::WorldToScreen(Dolly::GetPositionForCurrentTime(), windowWidth, windowHeight);
			//screenpos = Math::FixAspectRatio(screenpos, windowWidth, windowHeight);

			s_width = (width / (screenpos.z * 0.5));
			sh_width = s_width / 2.0f;
			outline_width = s_width + (sh_width / 2.0f);
			outline_width_h = outline_width / 2.0f;

			DrawFilled(screenpos.x - sh_width, screenpos.y - sh_width, s_width, s_width, 255, 0, 255, 255);
		}

	}
}

float centerW;
float centerH;

float borderW;

float textSpacing;

float rowHeight;

float menuHeight;

float CalcYForItem(int i)
{
	return (centerH - (menuHeight / 2)) + (fontHeight * i) + (textSpacing * (i + 1));
}

void DrawControls() {
	//float center = windowWidth / 2;
	//float half_center = center / 2;
	//float height = windowHeight / 10;

	//DrawFilled(center - half_center, 50, center, height, 46, 46, 46, 200);

	centerW = windowWidth / 2;
	centerH = windowHeight / 2;

	borderW = windowWidth / 50;

	textSpacing = windowHeight / 200;

	rowHeight = fontHeight + (textSpacing * 2);

	menuHeight = rowHeight * (UI_COUNT - 1);

	float widest_text = 0;
	//Get Widest piece of text:
	for (int i = 0; i < UI_COUNT; i++)
	{
		float w = 0;
		if (i == UI_TIMESCALE) {
			char str[40];

			sprintf(str, "%.2f", *Halo::timescale);
			std::string text = std::string(UI::GetName(i)) + ": " + std::string(str);
			w = GetTextWidth((char*)text.c_str(), dx_Font);
		}
		else {

			if (UI::DisplayError(i))
			{
				w = GetTextWidth((char*)UI::GetErrorText(i), dx_Font);
			}
			else {
				w = GetTextWidth((char*)UI::GetName(i), dx_Font);
			}
		}

		if (w > widest_text)
			widest_text = w;
	}

	float menuW = widest_text + (textSpacing * 2);

	//Draw Background
	DrawFilled(borderW, centerH - (menuHeight / 2), menuW, menuHeight, 64, 123, 165, 200);

	DrawShadowString((char*)"F2: Toggle Overlay", borderW, centerH - (menuHeight / 2) - fontHeight - textSpacing, 255, 255, 255, dx_Font);

	//Draw selected Item
	DrawFilled(borderW, CalcYForItem(UI::GetIndex()) - textSpacing, menuW, fontHeight + (textSpacing * 2), 125, 172, 205, 255);


	for (int i = 0; i < UI_COUNT; i++)
	{
		//Draw Each Item
		float Y = CalcYForItem(i);

		char* text = (char*)UI::GetName(i);

		if (!text)
			DrawShadowString((char*)"(NULL STRING)", borderW + textSpacing, Y, 255, 255, 255, dx_Font);
		else {

			//Special Case for Timescale
			if (i == UI_TIMESCALE) {
				char str[40];

				sprintf(str, "%.2f", *Halo::timescale);
				std::string text_s = std::string(UI::GetName(i)) + ": " + std::string(str);
				DrawShadowString((char*)text_s.c_str(), borderW + textSpacing, Y, 255, 255, 255, dx_Font);
			}
			else {
				if (UI::DisplayError(i))
				{
					DrawShadowString((char*)UI::GetErrorText(i), borderW + textSpacing, Y, 255, 255, 255, dx_Font);
				}
				else {
					DrawShadowString(text, borderW + textSpacing, Y, 255, 255, 255, dx_Font);
				}
			}
		}
	}

	// float total = 5;
	// float chunk = center / total;
	// 
	// float w = windowWidth / 40;
	// float wh = w / 2;
	// 
	// for (int i = 0; i < total; i += 1) {
	// 	float x = (center - half_center) + (chunk * i) - wh + (chunk / 2);
	// 	float y = 50 + (height / 2) - (w / 2);
	// 
	// 
	// 
	// 
	// 	if (i == 0)
	// 	{
	// 		char str[40];
	// 
	// 		// Print in string the value of var  
	// 		// with two decimal point 
	// 		sprintf(str, "%.2f", *Halo::timescale);
	// 
	// 		std::string text = std::string(str);
	// 		int width = GetTextWidth((char*)text.c_str(), dx_Font);
	// 		DrawShadowString((char*)text.c_str(), x - (width / 2), y, 255, 255, 255, dx_Font);
	// 	}
	// 	else {
	// 		DrawFilled(x, y, w, w, 255, 255, 255, 255);
	// 	}
	// }

}

int Render()
{
	dx_Device->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
	dx_Device->BeginScene();

	if (TargetWnd == GetForegroundWindow())
	{
		if (Settings::draw_camera_path)
		{
			if(!Dolly::IsDollying())
				DrawCameraMarkers();

			DrawControls();
			//Test shit


		//	float ratio = (float)windowHeight / (float)windowWidth;
		//
		//	for (float angle = 0; angle < Math::radians(360); angle += 0.1) {
		//		Vector3 pos;
		//		pos.x = 0.2;
		//		pos.y = 0.5;
		//
		//		pos = Math::RotatePointAroundCenter(pos, angle);
		//		pos = Math::FixAspectRatio(pos, windowWidth, windowHeight);
		//
		//		DrawFilled(pos.x * windowWidth, pos.y * windowHeight, 10, 10, 255, 255, 255, 255);
		//	}



		}
	}

	dx_Device->EndScene();
	dx_Device->PresentEx(0, 0, 0, 0, 0);

	return 0;
}