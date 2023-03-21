#pragma once

#define VC_EXTRALEAN 1
#define DIRECTINPUT_VERSION 0x0800

#include <Windows.h>
#include <guiddef.h>
#include <mmreg.h>
#include <dsound.h>
#include <dinput.h>
#include <Shlwapi.h>
#include <map>
#include <vector>
#include <set>
#include <mutex>
#include <filesystem>

#include "thirdparty/minhook/include/MinHook.h"
#include "WindowData.h"
#include "settings.h"
#include "utils.h"
