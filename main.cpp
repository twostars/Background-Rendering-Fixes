#include "stdafx.h"
#include "utils.h"
#include "hooks.h"

bool g_loaded = false;

static const std::set<std::wstring, ci_less> s_expectedFilenames =
{
	L"ed8" // Trails of Cold Steel 1
};

static const std::set<std::wstring> s_partialPaths =
{
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
	auto& path = GetExePath();
	wchar_t filename[_MAX_FNAME] = { 0 };
	_wsplitpath_s(path.c_str(), nullptr, 0, nullptr, 0, filename, _MAX_FNAME, nullptr, 0);

	// Ideally we should check the filename if it's unique enough.
	auto itr = s_expectedFilenames.find(filename);
	if (itr != s_expectedFilenames.end())
		return true;

	// Partial matches are good too, as a second resort.
	for (auto itr = s_partialPaths.begin(); itr != s_partialPaths.end(); ++itr)
	{
		if (StrStrIW(path.c_str(), (*itr).c_str()) != nullptr)
			return true;
	}

	return false;
}

void InitInstance(HANDLE hModule)
{
	if (g_loaded)
		return;

	InstallHooks();
	g_loaded = true;
}

void ExitInstance(HANDLE hModule)
{
	if (!g_loaded)
		return;

	RemoveHooks();
	g_loaded = false;
}
