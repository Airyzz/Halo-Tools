// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "StdInc.h"

HINSTANCE hModule;
int procId;

DWORD WINAPI Initialise(LPVOID Param) {

    procId = GetProcessId(hModule);
    AllocConsole();
    AttachConsole(procId);
    SetConsoleTitleA("Halo Director");


    Log::Info("Successfully Attached to Halo!");
    ConsoleCommands::Initialise();

    //Halo::Initialise();

    //Dolly::Initialise();
    Hooks::Initialise();
    return 0;
}


BOOL APIENTRY DllMain( HMODULE _hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        hModule = _hModule;
        CreateThread(NULL, 0, &Initialise, NULL, 0, NULL);
    }
    
    return TRUE;
}

