// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "StdInc.h"
#include "UI.h"
#include <iomanip>

int procId;

DWORD WINAPI Initialise(LPVOID Param) {

    procId = GetProcessId(Halo::hMod);
    AllocConsole();
    AttachConsole(procId);
    SetConsoleTitleA("Halo Director");

    Log::Info("Successfully Attached to Halo!");

    std::time_t result = std::time(nullptr);

    //1611974668 = January 30, 2020
    //if (result < 1611974668) {
    //    Log::Info("Current Time: %d", result);

    ConsoleCommands::Initialise();
    Halo::Initialise();
    Dolly::Initialise();
    UI::Init();
    Hooks::Initialise();
    Drawing::Init();

    //}
    //else {
    //    Log::Error("Beta has now expired. Please contact Airyz to request an extension, or wait until the final release");
    //}


    return 0;
}


BOOL APIENTRY DllMain( HMODULE _hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        Halo::hMod = _hModule;
        CreateThread(NULL, 0, &Initialise, NULL, 0, NULL);
    }
    
    return TRUE;
}

