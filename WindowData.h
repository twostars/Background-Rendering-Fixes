#pragma once

struct WindowData
{
	WNDPROC OriginalProc	= nullptr;
	bool AppActivateHandled	= false;
	bool ActivateHandled	= false;
	bool Focused			= false;
};
