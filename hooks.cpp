#include "stdafx.h"
#include "hooks.h"
#include "My_IDirectSound.h"
#include "My_IDirectSound8.h"

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

DETOUR_TRAMPOLINE(HWND WINAPI Real_CreateWindowExW(
	DWORD     dwExStyle,
	LPCWSTR   lpClassName,
	LPCWSTR   lpWindowName,
	DWORD     dwStyle,
	int       X,
	int       Y,
	int       nWidth,
	int       nHeight,
	HWND      hWndParent,
	HMENU     hMenu,
	HINSTANCE hInstance,
	LPVOID    lpParam
), CreateWindowExW);

DETOUR_TRAMPOLINE(BOOL WINAPI Real_DestroyWindow(
	HWND hWnd
), DestroyWindow);

DETOUR_TRAMPOLINE_EMPTY(HRESULT WINAPI Real_DirectSoundCreate(
	LPCGUID pcGuidDevice,
	LPDIRECTSOUND* ppDS,
	LPUNKNOWN pUnkOuter
));

DETOUR_TRAMPOLINE_EMPTY(HRESULT WINAPI Real_DirectSoundCreate8(
	LPCGUID pcGuidDevice,
	LPDIRECTSOUND8* ppDS,
	LPUNKNOWN pUnkOuter
));

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

HWND WINAPI hooked_CreateWindowExW(
	DWORD      dwExStyle,
	LPCWSTR    lpClassName,
	LPCWSTR    lpWindowName,
	DWORD      dwStyle,
	int        X,
	int        Y,
	int        nWidth,
	int        nHeight,
	HWND       hWndParent,
	HMENU      hMenu,
	HINSTANCE  hInstance,
	LPVOID     lpParam
);

BOOL WINAPI hooked_DestroyWindow(
	HWND hWnd
);

HRESULT WINAPI hooked_DirectSoundCreate(
	LPCGUID pcGuidDevice,
	LPDIRECTSOUND* ppDS,
	LPUNKNOWN pUnkOuter);

HRESULT WINAPI hooked_DirectSoundCreate8(
	LPCGUID pcGuidDevice,
	LPDIRECTSOUND8* ppDS,
	LPUNKNOWN pUnkOuter);

void InstallHooks()
{
	DetourFunctionWithTrampoline((PBYTE)Real_SetWindowLongPtrA, (PBYTE)hooked_SetWindowLongPtrA);
	DetourFunctionWithTrampoline((PBYTE)Real_SetWindowLongPtrW, (PBYTE)hooked_SetWindowLongPtrW);
	DetourFunctionWithTrampoline((PBYTE)Real_GetWindowLongPtrA, (PBYTE)hooked_GetWindowLongPtrA);
	DetourFunctionWithTrampoline((PBYTE)Real_GetWindowLongPtrW, (PBYTE)hooked_GetWindowLongPtrW);
	DetourFunctionWithTrampoline((PBYTE)Real_CreateWindowExW, (PBYTE)hooked_CreateWindowExW);
	DetourFunctionWithTrampoline((PBYTE)Real_DestroyWindow, (PBYTE)hooked_DestroyWindow);

	PBYTE fnDirectSoundCreate = DetourFindFunction("DSOUND", "DirectSoundCreate");
	if (fnDirectSoundCreate != nullptr)
		DetourFunctionWithEmptyTrampoline((PBYTE)Real_DirectSoundCreate, fnDirectSoundCreate, (PBYTE)hooked_DirectSoundCreate);

	PBYTE fnDirectSoundCreate8 = DetourFindFunction("DSOUND", "DirectSoundCreate8");
	if (fnDirectSoundCreate8 != nullptr)
		DetourFunctionWithEmptyTrampoline((PBYTE)Real_DirectSoundCreate8, fnDirectSoundCreate8, (PBYTE)hooked_DirectSoundCreate8);
}

void RemoveHooks()
{
	DetourRemoveWithTrampoline((PBYTE)Real_SetWindowLongPtrA, (PBYTE)hooked_SetWindowLongPtrA);
	DetourRemoveWithTrampoline((PBYTE)Real_SetWindowLongPtrW, (PBYTE)hooked_SetWindowLongPtrW);
	DetourRemoveWithTrampoline((PBYTE)Real_GetWindowLongPtrA, (PBYTE)hooked_GetWindowLongPtrA);
	DetourRemoveWithTrampoline((PBYTE)Real_GetWindowLongPtrW, (PBYTE)hooked_GetWindowLongPtrW);
	DetourRemoveWithTrampoline((PBYTE)Real_CreateWindowExW, (PBYTE)hooked_CreateWindowExW);
	DetourRemoveWithTrampoline((PBYTE)Real_DestroyWindow, (PBYTE)hooked_DestroyWindow);

	PBYTE fnDirectSoundCreate = DetourFindFunction("DSOUND", "DirectSoundCreate");
	if (fnDirectSoundCreate != nullptr)
		DetourRemoveWithTrampoline((PBYTE)Real_DirectSoundCreate, (PBYTE)hooked_DirectSoundCreate);

	PBYTE fnDirectSoundCreate8 = DetourFindFunction("DSOUND", "DirectSoundCreate8");
	if (fnDirectSoundCreate8 != nullptr)
		DetourRemoveWithTrampoline((PBYTE)Real_DirectSoundCreate8, (PBYTE)hooked_DirectSoundCreate8);
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

HWND WINAPI hooked_CreateWindowExW(
	DWORD      dwExStyle,
	LPCWSTR    lpClassName,
	LPCWSTR    lpWindowName,
	DWORD      dwStyle,
	int        X,
	int        Y,
	int        nWidth,
	int        nHeight,
	HWND       hWndParent,
	HMENU      hMenu,
	HINSTANCE  hInstance,
	LPVOID     lpParam
)
{
	HWND hWnd = Real_CreateWindowExW(
		dwExStyle,
		lpClassName,
		lpWindowName,
		dwStyle,
		X,
		Y,
		nWidth,
		nHeight,
		hWndParent,
		hMenu,
		hInstance,
		lpParam
	);

	if (hWnd != nullptr)
	{
		WNDPROC originalProc = (WNDPROC)GetWindowLongPtrW(hWnd, GWLP_WNDPROC);
		if (originalProc != nullptr)
		{
			std::lock_guard<std::recursive_mutex> lock(g_lock);
			g_windowData[hWnd].OriginalProc = originalProc;
			Real_SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (LONG_PTR)&WindowProc);
		}
	}

	return hWnd;
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

HRESULT WINAPI hooked_DirectSoundCreate(
	LPCGUID pcGuidDevice,
	LPDIRECTSOUND* ppDS,
	LPUNKNOWN pUnkOuter)
{
	HRESULT hr = Real_DirectSoundCreate(pcGuidDevice, ppDS, pUnkOuter);
	if (SUCCEEDED(hr) && ppDS != nullptr)
		*ppDS = new My_IDirectSound(*ppDS);
	return hr;
}

HRESULT WINAPI hooked_DirectSoundCreate8(
	LPCGUID pcGuidDevice,
	LPDIRECTSOUND8* ppDS,
	LPUNKNOWN pUnkOuter)
{
	HRESULT hr = Real_DirectSoundCreate8(pcGuidDevice, ppDS, pUnkOuter);
	if (SUCCEEDED(hr) && ppDS != nullptr)
		*ppDS = new My_IDirectSound8(*ppDS);
	return hr;
}
