#include "stdafx.h"
#include "hooks.h"

extern std::map<HWND, WindowData> g_windowData;
extern std::recursive_mutex g_lock;

LRESULT CALLBACK WindowProc(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
);

DETOUR_TRAMPOLINE(LONG_PTR WINAPI Real_SetWindowLongPtrA(
	HWND     hWnd,
	int      nIndex,
	LONG_PTR dwNewLong
), SetWindowLongPtrA);

DETOUR_TRAMPOLINE(LONG_PTR WINAPI Real_SetWindowLongPtrW(
	HWND     hWnd,
	int      nIndex,
	LONG_PTR dwNewLong
), SetWindowLongPtrW);

DETOUR_TRAMPOLINE(LONG_PTR WINAPI Real_GetWindowLongPtrA(
	HWND hWnd,
	int  nIndex
), GetWindowLongPtrA);

DETOUR_TRAMPOLINE(LONG_PTR WINAPI Real_GetWindowLongPtrW(
	HWND hWnd,
	int  nIndex
), GetWindowLongPtrW);

DETOUR_TRAMPOLINE(BOOL WINAPI Real_DestroyWindow(
	HWND hWnd
), DestroyWindow);

LONG_PTR WINAPI hooked_SetWindowLongPtrA(
	HWND     hWnd,
	int      nIndex,
	LONG_PTR dwNewLong
);

LONG_PTR WINAPI hooked_SetWindowLongPtrW(
	HWND     hWnd,
	int      nIndex,
	LONG_PTR dwNewLong
);

LONG_PTR WINAPI hooked_GetWindowLongPtrA(
	HWND hWnd,
	int  nIndex
);

LONG_PTR WINAPI hooked_GetWindowLongPtrW(
	HWND hWnd,
	int  nIndex
);

BOOL WINAPI hooked_DestroyWindow(
	HWND hWnd
);

void InstallHooks()
{
	DetourFunctionWithTrampoline((PBYTE)Real_SetWindowLongPtrA, (PBYTE)hooked_SetWindowLongPtrA);
	DetourFunctionWithTrampoline((PBYTE)Real_SetWindowLongPtrW, (PBYTE)hooked_SetWindowLongPtrW);
	DetourFunctionWithTrampoline((PBYTE)Real_GetWindowLongPtrA, (PBYTE)hooked_GetWindowLongPtrA);
	DetourFunctionWithTrampoline((PBYTE)Real_GetWindowLongPtrW, (PBYTE)hooked_GetWindowLongPtrW);
	DetourFunctionWithTrampoline((PBYTE)Real_DestroyWindow, (PBYTE)hooked_DestroyWindow);
}

void RemoveHooks()
{
	DetourRemoveWithTrampoline((PBYTE)Real_SetWindowLongPtrA, (PBYTE)hooked_SetWindowLongPtrA);
	DetourRemoveWithTrampoline((PBYTE)Real_SetWindowLongPtrW, (PBYTE)hooked_SetWindowLongPtrW);
	DetourRemoveWithTrampoline((PBYTE)Real_GetWindowLongPtrA, (PBYTE)hooked_GetWindowLongPtrA);
	DetourRemoveWithTrampoline((PBYTE)Real_GetWindowLongPtrW, (PBYTE)hooked_GetWindowLongPtrW);
	DetourRemoveWithTrampoline((PBYTE)Real_DestroyWindow, (PBYTE)hooked_DestroyWindow);
}

LONG_PTR WINAPI hooked_SetWindowLongPtrA(
	HWND     hWnd,
	int      nIndex,
	LONG_PTR dwNewLong
)
{
	if (nIndex == GWLP_WNDPROC)
	{
		std::lock_guard<std::recursive_mutex> lock(g_lock);
		g_windowData[hWnd].OriginalProc = (WNDPROC)dwNewLong;
		dwNewLong = (LONG_PTR)&WindowProc;
	}

	return Real_SetWindowLongPtrA(hWnd, nIndex, dwNewLong);
}

LONG_PTR WINAPI hooked_SetWindowLongPtrW(
	HWND     hWnd,
	int      nIndex,
	LONG_PTR dwNewLong
)
{
	if (nIndex == GWLP_WNDPROC)
	{
		std::lock_guard<std::recursive_mutex> lock(g_lock);
		g_windowData[hWnd].OriginalProc = (WNDPROC)dwNewLong;
		dwNewLong = (LONG_PTR)&WindowProc;
	}

	return Real_SetWindowLongPtrW(hWnd, nIndex, dwNewLong);
}

LONG_PTR WINAPI hooked_GetWindowLongPtrA(
	HWND hWnd,
	int  nIndex
)
{
	if (nIndex == GWLP_WNDPROC)
	{
		std::lock_guard<std::recursive_mutex> lock(g_lock);
		auto itr = g_windowData.find(hWnd);
		if (itr != g_windowData.end())
			return (LONG_PTR)itr->second.OriginalProc;
	}

	return Real_GetWindowLongPtrA(hWnd, nIndex);
}

LONG_PTR WINAPI hooked_GetWindowLongPtrW(
	HWND hWnd,
	int  nIndex
)
{
	if (nIndex == GWLP_WNDPROC)
	{
		std::lock_guard<std::recursive_mutex> lock(g_lock);
		auto itr = g_windowData.find(hWnd);
		if (itr != g_windowData.end())
			return (LONG_PTR)itr->second.OriginalProc;
	}

	return Real_GetWindowLongPtrW(hWnd, nIndex);
}

BOOL WINAPI hooked_DestroyWindow(
	HWND hWnd
)
{
	BOOL r = Real_DestroyWindow(hWnd);

	// Remove this immediately afterwards so WindowProc still knows what to call.
	{
		std::lock_guard<std::recursive_mutex> lock(g_lock);
		g_windowData.erase(hWnd);
	}

	return r;
}
