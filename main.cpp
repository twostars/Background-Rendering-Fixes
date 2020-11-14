#include "stdafx.h"
#include "utils.h"
#include "hooks.h"

bool g_loaded = false;

static const std::set<std::wstring, ci_less> s_expectedFilenames =
{
	L"ed6_win",			// The Legend of Heroes: Trails in the Sky FC
	L"ed6_win_DX9",		// The Legend of Heroes: Trails in the Sky FC
	L"ed6_win2",		// The Legend of Heroes: Trails in the Sky SC
	L"ed6_win2_DX9",	// The Legend of Heroes: Trails in the Sky SC
	L"ed6_win3",		// The Legend of Heroes: Trails in the Sky the 3rd
	L"ed6_win3_DX9",	// The Legend of Heroes: Trails in the Sky the 3rd
	L"ed8",				// The Legend of Heroes: Trails of Cold Steel I
	L"ed8jp",			// The Legend of Heroes: Trails of Cold Steel I
	L"ed8_2_PC_US",		// The Legend of Heroes: Trails of Cold Steel II
	L"ed8_2_PC_JP",		// The Legend of Heroes: Trails of Cold Steel II
	L"ed8_3_PC",		// The Legend of Heroes: Trails of Cold Steel III
	L"ed8_3_PC_JP",		// The Legend of Heroes: Trails of Cold Steel III
	L"ffxiiiimg"		// Final Fantasy XIII
};

bool IsApplicableProcess();
void InitInstance(HANDLE hModule);
void ExitInstance(HANDLE hModule);

int APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	lpReserved;

	// Unload the module if we don't care about hooking this process.
	// This is important when the DLL is loaded globally in all processes via AppInit.
	if (!IsApplicableProcess())
		return FALSE;

	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
		InitInstance(hModule);
	else if (ul_reason_for_call == DLL_PROCESS_DETACH)
		ExitInstance(hModule);

	return TRUE;
}

bool IsApplicableProcess()
{
	// Ideally we should check the filename if it's unique enough.
	return (s_expectedFilenames.find(GetProcessName()) != s_expectedFilenames.end());
}

void InitInstance(HANDLE hModule)
{
	if (g_loaded)
		return;

#if defined(_DEBUG)
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
#endif

	MH_Initialize();
	InstallHooks();
	g_loaded = true;
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
