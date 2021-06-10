#pragma once

#include "CombaseFunctions.h"

class ScopedHString
{
public:
	ScopedHString(const ScopedHString&) = delete;
	ScopedHString& operator=(const ScopedHString&) = delete;

	explicit ScopedHString(HSTRING hstr)
		: _hstr(hstr)
	{
	}

	explicit ScopedHString(const WCHAR* str) {
		assign(str, wcslen(str));
	}

	explicit ScopedHString(const WCHAR* str, size_t len) {
		assign(str, len);
	}

	void assign(const WCHAR* str, size_t len)
	{
		reset();
		CombaseFunctions::get()->WindowsCreateString(str, len, &_hstr);
	}

	HSTRING get() const {
		return _hstr;
	}

	void reset()
	{
		if (_hstr == nullptr)
			return;

		CombaseFunctions::get()->WindowsDeleteString(_hstr);
		_hstr = nullptr;
	}

	PCWSTR str(uint32_t* len) const {
		return CombaseFunctions::get()->WindowsGetStringRawBuffer(_hstr, len);
	}

	PCWSTR str() const
	{
		uint32_t len;
		return str(&len);
	}

	~ScopedHString() {
		reset();
	}

protected:
	HSTRING _hstr = nullptr;
};
