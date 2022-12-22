#include "Addresses.h"

//Current Playback Position while playing a demo (Bad Tickrate)
float* Halo::serverTime;

//Total time in seconds demo has been playing. Does not decrease when skipping backwards. Is affected by timescale (Good Tickrate)
float* Halo::serverSeconds;
float* Halo::fov;
DWORD64 Halo::CameraHookAddress;
Camera* Halo::Cam;
HWND Halo::pHwnd;
HMODULE Halo::hMod;

float* Halo::viewMatrix;
float* Halo::timescale;

struct handle_data {
    unsigned long process_id;
    HWND window_handle;
};


BOOL is_main_window(HWND handle)
{
    return GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle);
}

BOOL CALLBACK enum_windows_callback(HWND handle, LPARAM lParam)
{
    handle_data& data = *(handle_data*)lParam;
    unsigned long process_id = 0;
    GetWindowThreadProcessId(handle, &process_id);
    if (data.process_id != process_id || !is_main_window(handle))
        return TRUE;
    data.window_handle = handle;
    return FALSE;
}

HWND find_main_window(unsigned long process_id)
{
    handle_data data;
    data.process_id = process_id;
    data.window_handle = 0;
    EnumWindows(enum_windows_callback, (LPARAM)&data);
    return data.window_handle;
}

char* Halo::ScanIn(const char* pattern, const char* mask, char* begin, unsigned int size)
{
    unsigned int patternLength = strlen(mask);

    for (unsigned int i = 0; i < size - patternLength; i++)
    {
        bool found = true;
        for (unsigned int j = 0; j < patternLength; j++)
        {
            if (mask[j] != '?' && pattern[j] != *(begin + i + j))
            {
                found = false;
                break;
            }
        }
        if (found)
        {
            return (begin + i);
        }
    }
    return nullptr;
}

DWORD64 Halo::Scan(LPCWSTR modName, const char* pattern, const char* mask)
{
    HMODULE mod = GetModuleHandleW(modName);
    MODULEINFO info = MODULEINFO();
    GetModuleInformation(GetCurrentProcess(), mod, &info, sizeof(MODULEINFO));
    return (DWORD64)ScanIn(pattern, mask, (char*)info.lpBaseOfDll, info.SizeOfImage);
}

DWORD64 GetBaseAddress(LPCWSTR modName) {
    HMODULE mod = GetModuleHandleW(modName);
    MODULEINFO info = MODULEINFO();
    GetModuleInformation(GetCurrentProcess(), mod, &info, sizeof(MODULEINFO));
    return (DWORD64)info.lpBaseOfDll;
}

void Halo::Initialise() {
    //Find all addresses needed

    Log::Info("---------- Scanning for server time ----------");
    
    // This signature is located at: 18047418C in the old halo3_v100.ida64 file. Tested to be working in two different version of the game.
    // Hopefully the only thing that may change is the 0x1D Offset.
    DWORD64 temp = Scan(L"halo3.dll", "\x0f\x8f\x00\x00\x00\x00\xe8\x00\x00\x00\x00\x0f\x28\xc8", "xx????x????xxx") + 0x1D;
    
    if (temp < 0x1000)
    {
        Log::Error("Unable to find address: serverTime");
    }
    else
    {
        Log::Info("Found: %llx", temp);

        serverTime = (float*)(temp + 4 + *(int*)temp);

        Log::Info("Current Server Time: [%llx] -> %f", serverTime, *serverTime);
    }

    //viewMatrix = (float*)(GetBaseAddress(L"halo3.dll") + 0x28B1538);
    //Log::Info("View Matrix Address: %llx", viewMatrix);

    Log::Info("---------- Scanning for Server Seconds ----------");

    temp = Scan(L"halo3.dll", "\xf3\x0f\x11\x05\x00\x00\x00\x00\x48\x8b\x04\x10", "xxxx????xxxx") + 4;
    
    if (temp < 0x1000)
    {
        Log::Error("Unable to find address: serverSeconds");
    }
    else {
        Log::Info("Found Address: %llx", temp);
        serverSeconds = (float*)(temp + 4 + *(int*)temp);
        Log::Info("Calculated Address: %llx", serverSeconds);
    }

    Log::Info("---------- Scanning for Timescale ----------");

    temp = Scan(L"halo3.dll", "\xf3\x0f\x10\x05\x00\x00\x00\x00\x41\x0f\x2e\xc1\x7a", "xxxx????xxxxx") + 4;
    
    if (temp < 0x1000)
    {
        Log::Error("Unable to find address: timescale");
    }
    else {
        Log::Info("Found Address: %llx", temp);

        timescale = (float*)(temp + 4 + *(int*)temp);

        Log::Info("Calculated Address: %llx", timescale);
        Log::Info("Current Timescale: %f", *timescale);
    }

    Log::Info("---------- Scanning for FOV: ----------");

    //1801746C1 in 3065
    temp = Scan(L"halo3.dll",
        "\x48\x69\xc8\x00\x00\x00\x00\x48\x8d\x05\x00\x00\x00\x00\x48\x03\xc8\xe8\x00\x00\x00\x00\x84\xc0\x75\x00\x8a\x01\xc0\xe8\x00\xa8\x00\x74\x00\x48\x83\xc1\x00\x74\x00\x48\x83\x3d\x00\x00\x00\x00\x00\x74\x00\xf3\x0f\x10\x49\x00\x0f\x2e\x0d\x00\x00\x00\x00\x7a\x00\x74\x00\x0f\x28\xc1\xf3\x0f\x59\x05\x00\x00\x00\x00\xe8\x00\x00\x00\x00\x84\xc0\x75\x00\x0f\x28\xc2\x48\x83\xc4\x00\xc3 ",
        "xxx????xxx????xxxx????xxx?xxxx?x?x?xxx?x?xxx?????x?xxxx?xxx????x?x?xxxxxxx????x????xxx?xxxxxx?x") + 10;

    //~v2845 
    //temp = Scan(L"halo3.dll", 
    //    "\x48\x69\xc8\x00\x00\x00\x00\x48\x8d\x05\x00\x00\x00\x00\x48\x03\xc8\xe8\x00\x00\x00\x00\x84\xc0\x75\x00\x8a\x01\xc0\xe8\x00\xa8\x00\x74\x00\x48\x83\xc1\x00\x74\x00\x48\x83\x3d\x00\x00\x00\x00\x00\x74\x00\xf3\x0f\x10\x49\x00\x0f\x2e\x0d\x00\x00\x00\x00\x7a\x00\x74\x00\x0f\x28\xc1\xf3\x0f\x59\x05\x00\x00\x00\x00\xe8\x00\x00\x00\x00\x84\xc0\x75\x00\xf3\x0f\x10\x05\x00\x00\x00\x00\x48\x83\xc4\x00\xc3\xcc ", 
    //    "xxx????xxx????xxxx????xxx?xxxx?x?x?xxx?x?xxx?????x?xxxx?xxx????x?x?xxxxxxx????x????xxx?xxxx????xxx?xx") + 10;

    if (temp < 0x1000)
    {
        Log::Error("Unable to find address: FOV");
    }
    else {

        Log::Info("Found : %llx", temp);

        fov = (float*)(temp + 0x78 + *(int*)temp);

        Log::Info("Offset: %llx", *(int*)temp);
        Log::Info("Address calculated: %llx", fov);
        Log::Info("Current FOV: %f", *fov);
    }


    // Finding Camera Address in cheat engine:
    // Scan for 'f3 0f 59 ? ? ? ? ? f3 0f 11 ? ? f3 0f 59 ? f3 0f 58' (1803D4DB3) in halo3_v100.i64                 1 result only: ('f3 41 ? ? ? f3 0f 59 ? ? ? ? ? f3 0f 11 ? ? f3 0f 59 ? f3 0f 58')
    // Break and trace on 'movss [rsi+28],xmm0' camera coord is in xmm0
    Log::Info("---------- Scanning for Camera: ----------");

    //old pattern for ~2282 build
    //CameraHookAddress = Scan(L"halo3.dll", "\xf3\x41\x00\x00\x00\xf3\x0f\x59\x00\x00\x00\x00\x00\xf3\x0f\x11\x00\x00\xf3\x0f\x59\x00\xf3\x0f\x58", "xx???xxx?????xxx??xxx?xxx") + 13;

	//good as of 2845
    CameraHookAddress = Scan(L"halo3.dll", "\xf3\x0F\x00\x00\xf3\x0f\x11\x00\x00\xf3\x0f\x59\x00\xf3\x0f\x59\x00\xf3\x0f\x58", "xx??xxx??xxx?xxx?xxx") + 4;
    
    if (CameraHookAddress < 0x1000)
    {
        Log::Error("Unable to find address: CameraHookAddress");
    }
    else {
        Log::Info("Camera Hook Address: %llx", CameraHookAddress);
    }


    Log::Info("---------- Getting HWND -----------");
    pHwnd = find_main_window(GetCurrentProcessId());
    Log::Info("Hwnd: %llx", pHwnd);

    Log::Info("---------- Finished Reading Addresses ----------");
}