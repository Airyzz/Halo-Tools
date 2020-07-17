#include "Addresses.h"

float* Halo::serverTime;
float* Halo::serverSeconds;
float* Halo::fov;
World** Halo::world;

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

void Halo::Initialise() {
    DWORD64 temp = Scan(L"halo3.dll", "\xF3\x0F\x5F\xD1\xF3\x0F\x11\x05\x00\x00\x00\x00", "xxxxxxxx????") + 8;
    serverTime = (float*)(temp + 4 + *(int*)temp);
    serverSeconds = (float*)(0x24c + (DWORD64)serverTime);

    Log::Info("Server Seconds Address: %llx", serverSeconds);


    temp = Scan(L"halo3.dll", "\x48\x63\xC1\x48\x8D\x0D\x00\x00\x00\x00\x48\x69\xC0\x40\x0D\x00\x00", "xxxxxx????xxxxxxx") + 6;
    fov = (float*)(temp + 0x78 + *(int*)temp);

    temp = Scan(L"halo3.dll", "\x33\xD2\x48\x8D\x0D\x00\x00\x00\x00\x66\x90", "xxxxx????xx") + 5;
    world = (World**)(temp + 0x4C + *(int*)temp);


    //Log::Info("Found server time address: %llx", serverTime);
    //Log::Info("Found fov address: %llx", fov);
    //Log::Info("Found world address: %llx", world);
    //Log::Info("Found player address: %llx", (*world)->player);
    
    Log::Info("Current FOV: %f", *fov);
    Log::Info("Current Server Time: %f", *serverTime);
    Log::Info("Camera Position: %f, %f, %f", (*world)->player->camera.position.x, (*world)->player->camera.position.y, (*world)->player->camera.position.z);

    *fov = 1.0;

    while (*fov < 140.0) {
        *fov += 0.1;
        Sleep(1);
    }

}