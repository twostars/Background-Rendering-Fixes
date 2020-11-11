#pragma once

struct WindowData
{
	WNDPROC OriginalProc	= nullptr;
	bool Activated			= false;
};
