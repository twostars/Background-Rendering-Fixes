#pragma once

struct ci_less
{
	bool operator()(const std::wstring& str1, const std::wstring& str2) const;
};

const std::wstring& GetExePath();
