#include "stdafx.h"
#include "CombaseFunctions.h"

HMODULE CombaseFunctions::combase_dll_ = nullptr;
decltype(&::RoGetActivationFactory) CombaseFunctions::get_factory_func_ = nullptr;
decltype(&::WindowsCreateString) CombaseFunctions::create_string_func_ = nullptr;
decltype(&::WindowsDeleteString) CombaseFunctions::delete_string_func_ = nullptr;
decltype(&::WindowsGetStringRawBuffer) CombaseFunctions::get_string_raw_buffer_func_ = nullptr;

CombaseFunctions::CombaseFunctions()
{
	if (combase_dll_ == nullptr)
		combase_dll_ = LoadLibraryW(L"combase.dll");
}

CombaseFunctions::~CombaseFunctions()
{
	if (combase_dll_)
	{
		FreeLibrary(combase_dll_);
		combase_dll_ = nullptr;
	}
}

bool CombaseFunctions::LoadFunctions()
{
	if (!combase_dll_)
		return false;

	get_factory_func_ = reinterpret_cast<decltype(&::RoGetActivationFactory)>(
		GetProcAddress(combase_dll_, "RoGetActivationFactory"));
	if (!get_factory_func_)
		return false;

	create_string_func_ = reinterpret_cast<decltype(&::WindowsCreateString)>(
		GetProcAddress(combase_dll_, "WindowsCreateString"));
	if (!create_string_func_)
		return false;

	delete_string_func_ = reinterpret_cast<decltype(&::WindowsDeleteString)>(
		GetProcAddress(combase_dll_, "WindowsDeleteString"));
	if (!delete_string_func_)
		return false;

	get_string_raw_buffer_func_ =
		reinterpret_cast<decltype(&::WindowsGetStringRawBuffer)>(
			GetProcAddress(combase_dll_, "WindowsGetStringRawBuffer"));
	if (!get_string_raw_buffer_func_)
		return false;

	return true;
}


CombaseFunctions* CombaseFunctions::get()
{
	static CombaseFunctions _;
	static bool initialized = false;
	if (!initialized)
		initialized = _.LoadFunctions();

	return &_;
}
