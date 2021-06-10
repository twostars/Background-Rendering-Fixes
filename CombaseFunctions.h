#pragma once

#include <roapi.h>
#include <winstring.h>

// Provides access to functions in combase.dll which may not be available on
// Windows 7. Loads functions dynamically at runtime to prevent library
// dependencies.
class CombaseFunctions
{
public:
	CombaseFunctions();
	~CombaseFunctions();

	bool LoadFunctions();

	HRESULT RoGetActivationFactory(HSTRING class_id, const IID& iid, void** out_factory) {
		return get_factory_func_(class_id, iid, out_factory);
	}

	HRESULT WindowsCreateString(const WCHAR* src, uint32_t len, HSTRING* out_hstr) {
		return create_string_func_(src, len, out_hstr);
	}

	HRESULT WindowsDeleteString(HSTRING hstr) {
		return delete_string_func_(hstr);
	}

	const WCHAR* WindowsGetStringRawBuffer(HSTRING hstr, uint32_t* out_len) {
		return get_string_raw_buffer_func_(hstr, out_len);
	}

	static CombaseFunctions* get();

private:
	static HMODULE combase_dll_;
	static decltype(&::RoGetActivationFactory) get_factory_func_;
	static decltype(&::WindowsCreateString) create_string_func_;
	static decltype(&::WindowsDeleteString) delete_string_func_;
	static decltype(&::WindowsGetStringRawBuffer) get_string_raw_buffer_func_;
};
