#include "stdafx.h"
#include "utils.h"

bool ci_less::operator()(const std::wstring& str1, const std::wstring& str2) const
{
	return _wcsicmp(str1.c_str(), str2.c_str()) < 0;
}

const std::wstring& GetExePath()
{
	static bool loaded = false;
	static std::wstring path;

	if (!loaded)
	{
		std::vector<wchar_t> pathBuffer;
		DWORD copied = 0;
		do
		{
			pathBuffer.resize(pathBuffer.size() + MAX_PATH);
			copied = GetModuleFileNameW(0, pathBuffer.data(), pathBuffer.size());
		} while (copied >= pathBuffer.size());
		pathBuffer.resize(copied);

		path.assign(pathBuffer.begin(), pathBuffer.end());
	}

	return path;
}
