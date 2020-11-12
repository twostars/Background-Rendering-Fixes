#include "stdafx.h"
#include "utils.h"

std::map<HWND, WindowData> g_windowData;
bool g_applicationInFocus = true;
std::recursive_mutex g_lock;

LRESULT CALLBACK WindowProc(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	std::lock_guard<std::recursive_mutex> lock(g_lock);
	auto itr = g_windowData.find(hwnd);
	if (itr == g_windowData.end())
		return DefWindowProc(hwnd, uMsg, wParam, lParam);

	auto& windowData = itr->second;

	if (uMsg == WM_ACTIVATEAPP)
	{
		g_applicationInFocus = (wParam != 0);

		// We should exempt any app lost focus messages
		if (wParam == 0
			// or any re-activations.
			|| windowData.Activated)
			return 0;

		windowData.Activated = true;
	}

	// Restore the original call.
	return CallWindowProc(windowData.OriginalProc, hwnd, uMsg, wParam, lParam);
}
