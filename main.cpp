#include "stdafx.h"
#include "hooks.h"

bool g_loaded = false;
DWORD g_processId;

void InitInstance(HANDLE hModule);
void ExitInstance(HANDLE hModule);

int APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	lpReserved;

	// Unload the module if we don't care about hooking this process.
	// This is important when the DLL is loaded globally in all processes via AppInit.
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		auto& configPath = Settings::GetDefaultConfigPath();
		auto& processName = GetProcessName();
		if (!g_settings.Load(processName.c_str(), configPath.c_str()))
			return FALSE;

		InitInstance(hModule);
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH)
		ExitInstance(hModule);

	return TRUE;
}

void InitInstance(HANDLE hModule)
{
	if (g_loaded)
		return;

#if defined(_DEBUG)
	AllocConsole();
	(void)freopen("CONIN$", "r", stdin);
	(void)freopen("CONOUT$", "w", stdout);
	(void)freopen("CONOUT$", "w", stderr);
#endif

	MH_Initialize();
	InstallHooks();

	g_loaded = true;
	g_processId = GetCurrentProcessId();
}

void ExitInstance(HANDLE hModule)
{
	if (!g_loaded)
		return;

	RemoveHooks();
	MH_Uninitialize();

#if defined(_DEBUG)
	FreeConsole();
#endif

	g_loaded = false;
}
