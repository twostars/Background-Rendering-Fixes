#pragma once

#define VC_EXTRALEAN 1
#define WIN32_LEAN_AND_MEAN 1

#include <Windows.h>
#include <Shlwapi.h>
#include <map>
#include <vector>
#include <set>
#include <mutex>

#define DETOURS_INTERNAL 1
#include "detours/detours.h"

#include "WindowData.h"
