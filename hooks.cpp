#include "stdafx.h"
#include "hooks.h"
#include "My_IDirectSound.h"
#include "My_IDirectSound8.h"

LRESULT CALLBACK WindowProc(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
);

LONG_PTR (WINAPI *Real_SetWindowLongPtrA)(
	HWND     hWnd,
	int      nIndex,
	LONG_PTR dwNewLong
) = 0;

LONG_PTR (WINAPI *Real_SetWindowLongPtrW)(
	HWND     hWnd,
	int      nIndex,
	LONG_PTR dwNewLong
) = 0;

LONG_PTR (WINAPI *Real_GetWindowLongPtrA)(
	HWND hWnd,
	int  nIndex
) = 0;

LONG_PTR (WINAPI *Real_GetWindowLongPtrW)(
	HWND hWnd,
	int  nIndex
) = 0;

HWND (WINAPI *Real_CreateWindowExA)(
	DWORD     dwExStyle,
	LPCSTR    lpClassName,
	LPCSTR    lpWindowName,
	DWORD     dwStyle,
	int       X,
	int       Y,
	int       nWidth,
	int       nHeight,
	HWND      hWndParent,
	HMENU     hMenu,
	HINSTANCE hInstance,
	LPVOID    lpParam
) = 0;

HWND (WINAPI *Real_CreateWindowExW)(
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
) = 0;

BOOL (WINAPI *Real_DestroyWindow)(
	HWND hWnd
) = 0;

SHORT (WINAPI *Real_GetAsyncKeyState)(
	int vKey
) = 0;

SHORT (WINAPI *Real_GetKeyState)(
	int nVirtKey
) = 0;

HRESULT (WINAPI *Real_DirectSoundCreate)(
	LPCGUID pcGuidDevice,
	LPDIRECTSOUND* ppDS,
	LPUNKNOWN pUnkOuter
) = 0;

HRESULT (WINAPI *Real_DirectSoundCreate8)(
	LPCGUID pcGuidDevice,
	LPDIRECTSOUND8* ppDS,
	LPUNKNOWN pUnkOuter
) = 0;

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

HWND WINAPI hooked_CreateWindowExA(
	DWORD      dwExStyle,
	LPCSTR     lpClassName,
	LPCSTR     lpWindowName,
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

SHORT WINAPI hooked_GetAsyncKeyState(
	int vKey
);

SHORT WINAPI hooked_GetKeyState(
	int nVirtKey
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
	MH_CreateHook(SetWindowLongPtrA, hooked_SetWindowLongPtrA, (LPVOID*)&Real_SetWindowLongPtrA);
	MH_CreateHook(SetWindowLongPtrW, hooked_SetWindowLongPtrW, (LPVOID*)&Real_SetWindowLongPtrW);
	MH_CreateHook(GetWindowLongPtrA, hooked_GetWindowLongPtrA, (LPVOID*)&Real_GetWindowLongPtrA);
	MH_CreateHook(GetWindowLongPtrW, hooked_GetWindowLongPtrW, (LPVOID*)&Real_GetWindowLongPtrW);
	MH_CreateHook(CreateWindowExA, hooked_CreateWindowExA, (LPVOID*)&Real_CreateWindowExA);
	MH_CreateHook(CreateWindowExW, hooked_CreateWindowExW, (LPVOID*)&Real_CreateWindowExW);
	MH_CreateHook(DestroyWindow, hooked_DestroyWindow, (LPVOID*)&Real_DestroyWindow);
	MH_CreateHook(GetAsyncKeyState, hooked_GetAsyncKeyState, (LPVOID*)&Real_GetAsyncKeyState);
	MH_CreateHook(GetKeyState, hooked_GetKeyState, (LPVOID*)&Real_GetKeyState);

	FARPROC fnDirectSoundCreate = GetProcAddress(GetModuleHandle(L"DSOUND"), "DirectSoundCreate");
	if (fnDirectSoundCreate != nullptr)
		MH_CreateHook(fnDirectSoundCreate, hooked_DirectSoundCreate, (LPVOID*)&Real_DirectSoundCreate);

	FARPROC fnDirectSoundCreate8 = GetProcAddress(GetModuleHandle(L"DSOUND"), "DirectSoundCreate8");
	if (fnDirectSoundCreate8 != nullptr)
		MH_CreateHook(fnDirectSoundCreate8, hooked_DirectSoundCreate8, (LPVOID*)&Real_DirectSoundCreate8);
	
	MH_EnableHook(MH_ALL_HOOKS);
}

void RemoveHooks()
{
	MH_DisableHook(MH_ALL_HOOKS);
}

LONG_PTR WINAPI hooked_SetWindowLongPtrA(
	HWND     hWnd,
	int      nIndex,
	LONG_PTR dwNewLong
)
{
	if (nIndex == GWLP_WNDPROC
		&& dwNewLong != (LONG_PTR)WindowProc)
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
	if (nIndex == GWLP_WNDPROC
		&& dwNewLong != (LONG_PTR)WindowProc)
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

HWND WINAPI hooked_CreateWindowExA(
	DWORD      dwExStyle,
	LPCSTR     lpClassName,
	LPCSTR     lpWindowName,
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
	HWND hWnd = Real_CreateWindowExA(
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
		WNDPROC originalProc = (WNDPROC)GetWindowLongPtrA(hWnd, GWLP_WNDPROC);
		if (originalProc != nullptr
			&& originalProc != WindowProc)
		{
			std::lock_guard<std::recursive_mutex> lock(g_lock);
			g_windowData[hWnd].OriginalProc = originalProc;
			Real_SetWindowLongPtrA(hWnd, GWLP_WNDPROC, (LONG_PTR)&WindowProc);
		}
	}

	return hWnd;
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
		if (originalProc != nullptr
			&& originalProc != WindowProc)
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

SHORT WINAPI hooked_GetAsyncKeyState(int vKey)
{
	if (!g_applicationInFocus)
		return 0;

	return Real_GetAsyncKeyState(vKey);
}

SHORT WINAPI hooked_GetKeyState(int nVirtKey)
{
	if (!g_applicationInFocus)
		return 0;

	return Real_GetKeyState(nVirtKey);
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
