#include "stdafx.h"
#include "tls.h"

std::map<HWND, WindowData> g_windowData;
std::atomic<bool> g_applicationInFocus = true;
std::atomic<HWND> g_applicationWindow = nullptr;

// TODO: fetch this for non-DX11 games as required
std::atomic<DWORD> g_renderThreadId = 0;

std::recursive_mutex g_lock;

bool WindowProcImpl(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam,
	_In_ WindowData& windowData
);

LRESULT CALLBACK WndProcA(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	WriteLog(LOGLEVEL_DEBUG,
		L"WndProcA(%X, %X, %X, %X)\n",
		hwnd,
		uMsg,
		wParam,
		lParam);

	std::lock_guard<std::recursive_mutex> lock(g_lock);
	auto itr = g_windowData.find(hwnd);
	if (itr == g_windowData.end())
		return DefWindowProcA(hwnd, uMsg, wParam, lParam);

	auto& windowData = itr->second;
	if (!WindowProcImpl(hwnd, uMsg, wParam, lParam, windowData))
		return 0;

	// Restore the original call.
	return CallWindowProcA(windowData.OriginalProcA, hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK WndProcW(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	WriteLog(LOGLEVEL_DEBUG,
		L"WndProcW(%X, %X, %X, %X)\n",
		hwnd,
		uMsg,
		wParam,
		lParam);

	std::lock_guard<std::recursive_mutex> lock(g_lock);
	auto itr = g_windowData.find(hwnd);
	if (itr == g_windowData.end())
		return DefWindowProcW(hwnd, uMsg, wParam, lParam);

	auto& windowData = itr->second;
	if (!WindowProcImpl(hwnd, uMsg, wParam, lParam, windowData))
		return 0;

	// Restore the original call.
	return CallWindowProcW(windowData.OriginalProcW, hwnd, uMsg, wParam, lParam);
}

bool WindowProcImpl(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam,
	_In_ WindowData& windowData
)
{
	switch (uMsg)
	{
	case WM_ACTIVATEAPP:
		g_applicationInFocus = (wParam != 0);

		// We should exempt any app lost focus messages
		if (g_settings.UseBackgroundRendering
			|| g_settings.UseBackgroundAudio)
		{
			if (wParam == 0
				// or any re-activations.
				|| windowData.AppActivateHandled)
				return false;
		}

		windowData.AppActivateHandled = true;
		break;

	case WM_ACTIVATE:
		windowData.Focused = (wParam != 0);

		if (windowData.Focused
			&& hwnd == GetAncestor(hwnd, GA_ROOT))
			g_applicationWindow = hwnd;

		if (g_settings.UseBackgroundRendering
			|| g_settings.UseBackgroundAudio)
		{
			// We should exempt any app lost focus messages
			if (wParam == 0
				// or any re-activations.
				|| windowData.ActivateHandled)
				return false;
		}

		windowData.ActivateHandled = true;
		break;

	case WM_SETFOCUS:
	case WM_KILLFOCUS:
		if (g_settings.UseBackgroundRendering
			|| g_settings.UseBackgroundAudio)
		{
			if (hwnd == g_applicationWindow
				&& windowData.ActivateHandled)
				return false;
		}
		break;

	case WM_INPUT:
	{
		if ((g_settings.UseBackgroundRendering && !g_applicationInFocus)
			|| g_settings.DisableMouse)
		{
			UINT dwSize = sizeof(RAWINPUT);
			BYTE lpb[sizeof(RAWINPUT)];

			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));

			auto raw = (RAWINPUT*)lpb;
			if (raw->header.dwType == RIM_TYPEMOUSE)
				return false;
		}
	} break;

	default:
		if (uMsg >= WM_MOUSEFIRST
			&& uMsg <= WM_MOUSELAST)
		{
			if (g_settings.DisableMouse)
				return false;

			if (g_settings.UseBackgroundRendering
				&& !g_applicationInFocus)
				return false;
		}
		break;
	}

	return true;
}
