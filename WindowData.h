#pragma once

struct WindowData
{
	WNDPROC OriginalProcA	= nullptr;
	WNDPROC OriginalProcW	= nullptr;
	bool AppActivateHandled	= false;
	bool ActivateHandled	= false;
	bool Focused			= false;
};
