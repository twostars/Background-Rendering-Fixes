#include "stdafx.h"
#include "utils.h"

std::map<HWND, WindowData> g_windowDataA;
std::map<HWND, WindowData> g_windowDataW;

bool g_applicationInFocus = true;
HWND g_applicationWindow = nullptr;

std::recursive_mutex g_lock;

LRESULT CALLBACK WindowProcA(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	std::lock_guard<std::recursive_mutex> lock(g_lock);
	auto itr = g_windowDataA.find(hwnd);
	if (itr == g_windowDataA.end())
		return DefWindowProcA(hwnd, uMsg, wParam, lParam);

	auto& windowData = itr->second;

	if (uMsg == WM_ACTIVATEAPP)
	{
		g_applicationInFocus = (wParam != 0);

		// We should exempt any app lost focus messages
		if (wParam == 0
			// or any re-activations.
			|| windowData.AppActivateHandled)
			return 0;

		windowData.AppActivateHandled = true;
	}
	else if (uMsg == WM_ACTIVATE)
	{
		windowData.Focused = (wParam != 0);

		if (windowData.Focused
			&& hwnd == GetAncestor(hwnd, GA_ROOT))
			g_applicationWindow = hwnd;

		// We should exempt any app lost focus messages
		if (wParam == 0
			// or any re-activations.
			|| windowData.ActivateHandled)
			return 0;

		windowData.ActivateHandled = true;
	}
	else if (uMsg == WM_SETFOCUS
		|| uMsg == WM_KILLFOCUS)
	{
		if (hwnd == g_applicationWindow
			&& windowData.ActivateHandled)
			return 0;
	}

	// Restore the original call.
	return CallWindowProcA(windowData.OriginalProc, hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK WindowProcW(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	std::lock_guard<std::recursive_mutex> lock(g_lock);
	auto itr = g_windowDataW.find(hwnd);
	if (itr == g_windowDataW.end())
		return DefWindowProcW(hwnd, uMsg, wParam, lParam);

	auto& windowData = itr->second;

	if (uMsg == WM_ACTIVATEAPP)
	{
		g_applicationInFocus = (wParam != 0);

		// We should exempt any app lost focus messages
		if (wParam == 0
			// or any re-activations.
			|| windowData.AppActivateHandled)
			return 0;

		windowData.AppActivateHandled = true;
	}
	else if (uMsg == WM_ACTIVATE)
	{
		windowData.Focused = (wParam != 0);

		if (windowData.Focused
			&& hwnd == GetAncestor(hwnd, GA_ROOT))
			g_applicationWindow = hwnd;

		// We should exempt any app lost focus messages
		if (wParam == 0
			// or any re-activations.
			|| windowData.ActivateHandled)
			return 0;

		windowData.ActivateHandled = true;
	}
	else if (uMsg == WM_KILLFOCUS)
	{
		if (hwnd == g_applicationWindow)
			return 0;
	}

	// Restore the original call.
	return CallWindowProcW(windowData.OriginalProc, hwnd, uMsg, wParam, lParam);
}
