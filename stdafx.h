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

extern std::map<HWND, WindowData> g_windowDataA;
extern std::map<HWND, WindowData> g_windowDataW;
extern bool g_applicationInFocus;
extern HWND g_applicationWindow;
extern std::recursive_mutex g_lock;
