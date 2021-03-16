#include "stdafx.h"
#include "hooks.h"
#include "My_IDirectSound.h"
#include "My_IDirectSound8.h"
#include "My_IXAudio20.h"
#include "utils.h"

LRESULT CALLBACK WindowProcA(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
);

LRESULT CALLBACK WindowProcW(
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

HWND (*Real_GetActiveWindow)() = 0;
HWND (*Real_GetForegroundWindow)() = 0;

SHORT (WINAPI *Real_GetAsyncKeyState)(
	int vKey
) = 0;

SHORT (WINAPI *Real_GetKeyState)(
	int nVirtKey
) = 0;

BOOL (WINAPI *Real_GetKeyboardState)(
	PBYTE lpKeyState
) = 0;

BOOL (WINAPI *Real_ClipCursor)(
	const RECT* lpRect
) = 0;

BOOL (WINAPI *Real_SetCursorPos)(
	int X,
	int Y
) = 0;

HRESULT (WINAPI *Real_CoCreateInstance)(
	REFCLSID  rclsid,
	LPUNKNOWN pUnkOuter,
	DWORD     dwClsContext,
	REFIID    riid,
	LPVOID*   ppv
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

HWND hooked_GetActiveWindow();
HWND hooked_GetForegroundWindow();

SHORT WINAPI hooked_GetAsyncKeyState(
	int vKey
);

SHORT WINAPI hooked_GetKeyState(
	int nVirtKey
);

BOOL WINAPI hooked_GetKeyboardState(
	PBYTE lpKeyState
);

BOOL WINAPI hooked_ClipCursor(
	const RECT* lpRect
);

BOOL WINAPI hooked_SetCursorPos(
	int X,
	int Y
);

HRESULT WINAPI hooked_CoCreateInstance(
	REFCLSID  rclsid,
	LPUNKNOWN pUnkOuter,
	DWORD     dwClsContext,
	REFIID    riid,
	LPVOID*   ppv
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
	if (g_settings.UseBackgroundRendering)
	{
		MH_CreateHook(SetWindowLongPtrA, hooked_SetWindowLongPtrA, (LPVOID*)&Real_SetWindowLongPtrA);
		MH_CreateHook(SetWindowLongPtrW, hooked_SetWindowLongPtrW, (LPVOID*)&Real_SetWindowLongPtrW);
		MH_CreateHook(GetWindowLongPtrA, hooked_GetWindowLongPtrA, (LPVOID*)&Real_GetWindowLongPtrA);
		MH_CreateHook(GetWindowLongPtrW, hooked_GetWindowLongPtrW, (LPVOID*)&Real_GetWindowLongPtrW);
		MH_CreateHook(CreateWindowExA, hooked_CreateWindowExA, (LPVOID*)&Real_CreateWindowExA);
		MH_CreateHook(CreateWindowExW, hooked_CreateWindowExW, (LPVOID*)&Real_CreateWindowExW);
		MH_CreateHook(DestroyWindow, hooked_DestroyWindow, (LPVOID*)&Real_DestroyWindow);
		MH_CreateHook(GetActiveWindow, hooked_GetActiveWindow, (LPVOID*)&Real_GetActiveWindow);
		MH_CreateHook(GetForegroundWindow, hooked_GetForegroundWindow, (LPVOID*)&Real_GetForegroundWindow);
		MH_CreateHook(GetAsyncKeyState, hooked_GetAsyncKeyState, (LPVOID*)&Real_GetAsyncKeyState);
		MH_CreateHook(GetKeyState, hooked_GetKeyState, (LPVOID*)&Real_GetKeyState);
		MH_CreateHook(GetKeyboardState, hooked_GetKeyboardState, (LPVOID*)&Real_GetKeyboardState);
	}

	if (g_settings.UseBackgroundAudio)
	{
		HMODULE dsoundModule = GetModuleHandleW(L"DSOUND");
		if (dsoundModule != nullptr)
		{
			FARPROC fnDirectSoundCreate = GetProcAddress(dsoundModule, "DirectSoundCreate");
			if (fnDirectSoundCreate != nullptr)
				MH_CreateHook(fnDirectSoundCreate, hooked_DirectSoundCreate, (LPVOID*)&Real_DirectSoundCreate);

			FARPROC fnDirectSoundCreate8 = GetProcAddress(dsoundModule, "DirectSoundCreate8");
			if (fnDirectSoundCreate8 != nullptr)
				MH_CreateHook(fnDirectSoundCreate8, hooked_DirectSoundCreate8, (LPVOID*)&Real_DirectSoundCreate8);
		}
	}

	if (g_settings.UseAppAudioDevice)
		MH_CreateHook(CoCreateInstance, hooked_CoCreateInstance, (LPVOID*)&Real_CoCreateInstance);

	if (g_settings.UseUnclippedCursor)
	{
		MH_CreateHook(ClipCursor, hooked_ClipCursor, (LPVOID*)&Real_ClipCursor);
		MH_CreateHook(SetCursorPos, hooked_SetCursorPos, (LPVOID*)&Real_SetCursorPos);
	}

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
		&& dwNewLong != (LONG_PTR)WindowProcA)
	{
		std::lock_guard<std::recursive_mutex> lock(g_lock);
		g_windowDataA[hWnd].OriginalProc = (WNDPROC)dwNewLong;
		dwNewLong = (LONG_PTR)&WindowProcA;
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
		&& dwNewLong != (LONG_PTR)WindowProcW)
	{
		std::lock_guard<std::recursive_mutex> lock(g_lock);
		g_windowDataW[hWnd].OriginalProc = (WNDPROC)dwNewLong;
		dwNewLong = (LONG_PTR)&WindowProcW;
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
		auto itr = g_windowDataA.find(hWnd);
		if (itr != g_windowDataA.end())
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
		auto itr = g_windowDataW.find(hWnd);
		if (itr != g_windowDataW.end())
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
			&& originalProc != WindowProcA)
		{
			std::lock_guard<std::recursive_mutex> lock(g_lock);
			g_windowDataA[hWnd].OriginalProc = originalProc;
			Real_SetWindowLongPtrA(hWnd, GWLP_WNDPROC, (LONG_PTR)&WindowProcA);
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
			&& originalProc != WindowProcW)
		{
			std::lock_guard<std::recursive_mutex> lock(g_lock);
			g_windowDataW[hWnd].OriginalProc = originalProc;
			Real_SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (LONG_PTR)&WindowProcW);
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
		g_windowDataA.erase(hWnd);
		g_windowDataW.erase(hWnd);
	}

	return r;
}

HWND hooked_GetActiveWindow()
{
	if (!g_applicationInFocus)
		return g_applicationWindow;

	return Real_GetActiveWindow();
}

HWND hooked_GetForegroundWindow()
{
	if (!g_applicationInFocus)
		return g_applicationWindow;

	return Real_GetForegroundWindow();
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

BOOL WINAPI hooked_GetKeyboardState(PBYTE lpKeyState)
{
	if (!g_applicationInFocus)
	{
		// MSDN states this is a 256 byte array.
		memset(lpKeyState, 0, 256);
		return TRUE;
	}

	return Real_GetKeyboardState(lpKeyState);
}

BOOL WINAPI hooked_ClipCursor(const RECT* lpRect)
{
	if (!g_applicationInFocus
		|| g_settings.UseUnclippedCursor)
		return Real_ClipCursor(nullptr);

	return Real_ClipCursor(lpRect);
}

BOOL WINAPI hooked_SetCursorPos(int X, int Y)
{
	if (!g_applicationInFocus
		|| g_settings.UseUnclippedCursor)
		return TRUE;

	return Real_SetCursorPos(X, Y);
}

HRESULT WINAPI hooked_CoCreateInstance(
	REFCLSID  rclsid,
	LPUNKNOWN pUnkOuter,
	DWORD     dwClsContext,
	REFIID    riid,
	LPVOID*   ppv
)
{
	HRESULT hr = Real_CoCreateInstance(rclsid, pUnkOuter, dwClsContext, riid, ppv);
	if (SUCCEEDED(hr))
	{
		if (riid == IID_IXAudio2)
		{
			if (rclsid == CLSID_XAudio20
				|| rclsid == CLSID_XAudio20_Debug
				|| rclsid == CLSID_XAudio21
				|| rclsid == CLSID_XAudio21_Debug
				|| rclsid == CLSID_XAudio22
				|| rclsid == CLSID_XAudio22_Debug
				|| rclsid == CLSID_XAudio23
				|| rclsid == CLSID_XAudio23_Debug
				|| rclsid == CLSID_XAudio24
				|| rclsid == CLSID_XAudio24_Debug
				|| rclsid == CLSID_XAudio25
				|| rclsid == CLSID_XAudio25_Debug
				|| rclsid == CLSID_XAudio26
				|| rclsid == CLSID_XAudio26_Debug
				|| rclsid == CLSID_XAudio27
				|| rclsid == CLSID_XAudio27_Debug)
				*ppv = new My_IXAudio20((IXAudio20*)*ppv);
		}
	}

	return hr;
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
