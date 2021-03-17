#pragma once

#define VC_EXTRALEAN 1

#include <Windows.h>
#include <guiddef.h>
#include <mmreg.h>
#include <dsound.h>
#include <Shlwapi.h>
#include <map>
#include <vector>
#include <set>
#include <mutex>
#include <filesystem>

#include "thirdparty/minhook/include/MinHook.h"
#include "WindowData.h"
#include "settings.h"

extern std::map<HWND, WindowData> g_windowData;
extern std::atomic<bool> g_applicationInFocus;
extern std::atomic<HWND> g_applicationWindow;
extern std::atomic<DWORD> g_renderThreadId;
extern std::recursive_mutex g_lock;
