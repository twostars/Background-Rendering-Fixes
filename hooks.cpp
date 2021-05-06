#include "stdafx.h"
#include "hooks.h"
#include "My_IDirectSound.h"
#include "My_IDirectSound8.h"
#include "My_IDirectInput8.h"
#include "My_IXAudio20.h"
#include "tls.h"

#include <d3dcommon.h>

struct IDXGIAdapter;
struct DXGI_SWAP_CHAIN_DESC;
struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;

LRESULT CALLBACK WndProcA(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
);

LRESULT CALLBACK WndProcW(
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

BOOL (WINAPI *Real_GetCursorPos)(
	LPPOINT lpPoint
) = 0;

void (WINAPI *Real_Sleep)(
	DWORD dwMilliseconds
) = 0;

void (WINAPI *Real_RaiseException)(
	DWORD           dwExceptionCode,
	DWORD           dwExceptionFlags,
	DWORD           nNumberOfArguments,
	const ULONG_PTR* lpArguments
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

HRESULT (WINAPI *Real_DirectInput8Create)(
	HINSTANCE hinst,
	DWORD dwVersion,
	REFIID riidltf,
	LPVOID* ppvOut,
	LPUNKNOWN punkOuter) = 0;

HRESULT (WINAPI *Real_D3D11CreateDeviceAndSwapChain)(IDXGIAdapter          *pAdapter,
                                      D3D_DRIVER_TYPE        DriverType,
                                      HMODULE                Software,
                                      UINT                   Flags,
 _In_reads_opt_ (FeatureLevels) CONST D3D_FEATURE_LEVEL     *pFeatureLevels,
                                      UINT                   FeatureLevels,
                                      UINT                   SDKVersion,
 _In_opt_                       CONST DXGI_SWAP_CHAIN_DESC  *pSwapChainDesc,
 _Out_opt_                            IDXGISwapChain       **ppSwapChain,
 _Out_opt_                            ID3D11Device         **ppDevice,
 _Out_opt_                            D3D_FEATURE_LEVEL     *pFeatureLevel,
 _Out_opt_                            ID3D11DeviceContext  **ppImmediateContext) = 0;

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

BOOL WINAPI hooked_GetCursorPos(
	LPPOINT lpPoint
);

void WINAPI hooked_Sleep(
	DWORD dwMilliseconds
);

void WINAPI hooked_RaiseException(
	DWORD           dwExceptionCode,
	DWORD           dwExceptionFlags,
	DWORD           nNumberOfArguments,
	const ULONG_PTR* lpArguments
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

HRESULT WINAPI hooked_DirectInput8Create(
	HINSTANCE hinst,
	DWORD dwVersion,
	REFIID riidltf,
	LPVOID* ppvOut,
	LPUNKNOWN punkOuter);

HRESULT WINAPI hooked_D3D11CreateDeviceAndSwapChain(IDXGIAdapter          *pAdapter,
                                      D3D_DRIVER_TYPE        DriverType,
                                      HMODULE                Software,
                                      UINT                   Flags,
 _In_reads_opt_ (FeatureLevels) CONST D3D_FEATURE_LEVEL     *pFeatureLevels,
                                      UINT                   FeatureLevels,
                                      UINT                   SDKVersion,
 _In_opt_                       CONST DXGI_SWAP_CHAIN_DESC  *pSwapChainDesc,
 _Out_opt_                            IDXGISwapChain       **ppSwapChain,
 _Out_opt_                            ID3D11Device         **ppDevice,
 _Out_opt_                            D3D_FEATURE_LEVEL     *pFeatureLevel,
 _Out_opt_                            ID3D11DeviceContext  **ppImmediateContext);

void InstallHooks()
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

	if (g_settings.HookDirectSound)
	{
		HMODULE dsoundModule = LoadLibraryW(L"DSOUND");
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

	if (g_settings.HookDirectInput)
	{
		HMODULE dinputModule = LoadLibraryW(L"DINPUT8");
		if (dinputModule != nullptr)
		{
			FARPROC fnDirectInput8Create = GetProcAddress(dinputModule, "DirectInput8Create");
			if (fnDirectInput8Create != nullptr)
				MH_CreateHook(fnDirectInput8Create, hooked_DirectInput8Create, (LPVOID*)&Real_DirectInput8Create);
		}
	}

	MH_CreateHook(CoCreateInstance, hooked_CoCreateInstance, (LPVOID*)&Real_CoCreateInstance);
	MH_CreateHook(ClipCursor, hooked_ClipCursor, (LPVOID*)&Real_ClipCursor);
	MH_CreateHook(SetCursorPos, hooked_SetCursorPos, (LPVOID*)&Real_SetCursorPos);
	MH_CreateHook(GetCursorPos, hooked_GetCursorPos, (LPVOID*)&Real_GetCursorPos);
	MH_CreateHook(Sleep, hooked_Sleep, (LPVOID*)&Real_Sleep);

	HMODULE d3d11Module = LoadLibraryW(L"d3d11");
	if (d3d11Module != nullptr)
	{
		FARPROC fnD3D11CreateDeviceAndSwapChain = GetProcAddress(d3d11Module, "D3D11CreateDeviceAndSwapChain");
		if (fnD3D11CreateDeviceAndSwapChain != nullptr)
			MH_CreateHook(fnD3D11CreateDeviceAndSwapChain, hooked_D3D11CreateDeviceAndSwapChain, (LPVOID*)&Real_D3D11CreateDeviceAndSwapChain);
	}

	MH_CreateHook(RaiseException, hooked_RaiseException, (LPVOID*)&Real_RaiseException);
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
		&& dwNewLong != (LONG_PTR)WndProcA)
	{
		std::lock_guard<std::recursive_mutex> lock(g_lock);
		auto itr = g_windowData.find(hWnd);
		if (itr == g_windowData.end()
			|| itr->second.OriginalProcA == nullptr)
		{
			g_windowData[hWnd].OriginalProcA = (WNDPROC)dwNewLong;
			dwNewLong = (LONG_PTR)&WndProcA;
		}
		else
		{
			return (LONG_PTR)&WndProcA;
		}
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
		&& dwNewLong != (LONG_PTR)WndProcW)
	{
		std::lock_guard<std::recursive_mutex> lock(g_lock);
		auto itr = g_windowData.find(hWnd);
		if (itr == g_windowData.end()
			|| itr->second.OriginalProcW == nullptr)
		{
			g_windowData[hWnd].OriginalProcW = (WNDPROC)dwNewLong;
			dwNewLong = (LONG_PTR)&WndProcW;
		}
		else
		{
			return (LONG_PTR)&WndProcW;
		}
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
			return (LONG_PTR)itr->second.OriginalProcA;
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
			return (LONG_PTR)itr->second.OriginalProcW;
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
			&& originalProc != WndProcA)
		{
			std::lock_guard<std::recursive_mutex> lock(g_lock);
			g_windowData[hWnd].OriginalProcA = originalProc;
			Real_SetWindowLongPtrA(hWnd, GWLP_WNDPROC, (LONG_PTR)&WndProcA);
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
			&& originalProc != WndProcW)
		{
			std::lock_guard<std::recursive_mutex> lock(g_lock);
			g_windowData[hWnd].OriginalProcW = originalProc;
			Real_SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (LONG_PTR)&WndProcW);
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

HWND hooked_GetActiveWindow()
{
	if (g_settings.UseBackgroundRendering
		|| g_settings.UseBackgroundAudio)
	{
		if (!g_applicationInFocus)
			return g_applicationWindow;
	}

	return Real_GetActiveWindow();
}

HWND hooked_GetForegroundWindow()
{
	if (g_settings.UseBackgroundRendering
		|| g_settings.UseBackgroundAudio)
	{
		if (!g_applicationInFocus)
			return g_applicationWindow;
	}

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
	if (g_settings.UseBackgroundRendering
		&& !g_applicationInFocus)
		return 0;

	return Real_GetKeyState(nVirtKey);
}

BOOL WINAPI hooked_GetKeyboardState(PBYTE lpKeyState)
{
	if (g_settings.UseBackgroundRendering
		&& !g_applicationInFocus)
	{
		// MSDN states this is a 256 byte array.
		memset(lpKeyState, 0, 256);
		return TRUE;
	}

	return Real_GetKeyboardState(lpKeyState);
}

BOOL WINAPI hooked_ClipCursor(const RECT* lpRect)
{
	if ((g_settings.UseBackgroundRendering && !g_applicationInFocus)
		|| g_settings.UseUnclippedCursor)
		return Real_ClipCursor(nullptr);

	return Real_ClipCursor(lpRect);
}

BOOL WINAPI hooked_SetCursorPos(int X, int Y)
{
	if ((g_settings.UseBackgroundRendering && !g_applicationInFocus)
		|| g_settings.UseUnclippedCursor)
		return Real_ClipCursor(nullptr);

	return Real_SetCursorPos(X, Y);
}

BOOL WINAPI hooked_GetCursorPos(LPPOINT lpPoint)
{
	if (lpPoint == nullptr)
		return FALSE;

	static POINT ptLast = { 0 };

	if (g_settings.UseBackgroundRendering
		&& !g_applicationInFocus)
	{
		*lpPoint = ptLast;
		return TRUE;
	}

	BOOL r = Real_GetCursorPos(lpPoint);
	if (r)
		ptLast = *lpPoint;

	return r;
}

void WINAPI hooked_Sleep(
	DWORD dwMilliseconds
)
{
	// Tales of Vesperia microstutter hackfix
	if (g_settings.TalesOfVesperia_MicroStutterFix)
	{
		static const thread_local bool IsPollerThread =
			(g_tls.ThreadName == "WorkThread" || g_tls.ThreadName == "BusyThread");

		if (IsPollerThread
			|| g_tls.ThreadID == g_renderThreadId)
		{
			YieldProcessor();
			return;
		}
	}

	return Real_Sleep(dwMilliseconds);
}

void WINAPI hooked_RaiseException(
	DWORD           dwExceptionCode,
	DWORD           dwExceptionFlags,
	DWORD           nNumberOfArguments,
	const ULONG_PTR* lpArguments
)
{
	constexpr DWORD MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push,8)
	typedef struct tagTHREADNAME_INFO
	{
		DWORD dwType;		// Must be 0x1000.
		LPCSTR szName;		// Pointer to name (in user addr space).
		DWORD dwThreadID;	// Thread ID (-1=caller thread).
		DWORD dwFlags;		// Reserved for future use, must be zero.
	} THREADNAME_INFO;
#pragma pack(pop)

	if (dwExceptionCode == MS_VC_EXCEPTION)
	{
		auto info = (THREADNAME_INFO*)lpArguments;
		if (info->szName != nullptr
			&& info->dwFlags == 0
			&& info->dwType == 0x1000)
		{
			DWORD dwThreadId = info->dwThreadID;
			if (dwThreadId == -1)
				dwThreadId = g_tls.ThreadID;

			g_tls.ThreadName = info->szName;
		}
	}

	return Real_RaiseException(dwExceptionCode, dwExceptionFlags, nNumberOfArguments, lpArguments);
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

HRESULT WINAPI hooked_DirectInput8Create(
	HINSTANCE hinst,
	DWORD dwVersion,
	REFIID riidltf,
	LPVOID* ppvOut,
	LPUNKNOWN punkOuter)
{
	HRESULT hr = Real_DirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);
	if (SUCCEEDED(hr) && ppvOut != nullptr)
	{
		if (riidltf == IID_IDirectInput8A)
			*ppvOut = new My_IDirectInput8A(static_cast<IDirectInput8A*>(*ppvOut));
		else if (riidltf == IID_IDirectInput8W)
			*ppvOut = new My_IDirectInput8W(static_cast<IDirectInput8W*>(*ppvOut));
	}
	return hr;
}

HRESULT WINAPI hooked_D3D11CreateDeviceAndSwapChain(IDXGIAdapter* pAdapter,
	D3D_DRIVER_TYPE        DriverType,
	HMODULE                Software,
	UINT                   Flags,
	_In_reads_opt_(FeatureLevels) CONST D3D_FEATURE_LEVEL* pFeatureLevels,
	UINT                   FeatureLevels,
	UINT                   SDKVersion,
	_In_opt_                       CONST DXGI_SWAP_CHAIN_DESC* pSwapChainDesc,
	_Out_opt_                            IDXGISwapChain** ppSwapChain,
	_Out_opt_                            ID3D11Device** ppDevice,
	_Out_opt_                            D3D_FEATURE_LEVEL* pFeatureLevel,
	_Out_opt_                            ID3D11DeviceContext** ppImmediateContext)
{
	g_renderThreadId = g_tls.ThreadID;

	return Real_D3D11CreateDeviceAndSwapChain(
		pAdapter,
		DriverType,
		Software,
		Flags,
		pFeatureLevels,
		FeatureLevels,
		SDKVersion,
		pSwapChainDesc,
		ppSwapChain,
		ppDevice,
		pFeatureLevel,
		ppImmediateContext);
}
