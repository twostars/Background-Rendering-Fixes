#include "stdafx.h"

#include <roapi.h>
#include <winstring.h>
#include <mmdeviceapi.h>
#define INITGUID
#include <guiddef.h>

// {2a59116d-6c4f-45e0-a74f-707e3fef9258}
DEFINE_GUID(IID_IAudioPolicyConfig, 0x2a59116d, 0x6c4f, 0x45e0, 0xa7, 0x4f, 0x70, 0x7e, 0x3f, 0xef, 0x92, 0x58);

// {E6327CAD-DCEC-4949-AE8A-991E976A79D2}
DEFINE_GUID(DEVINTERFACE_AUDIO_RENDER, 0xe6327cad, 0xdcec, 0x4949, 0xae, 0x8a, 0x99, 0x1e, 0x97, 0x6a, 0x79, 0xd2);

// {2EEF81BE-33FA-4800-9670-1CD474972C3F}
DEFINE_GUID(DEVINTERFACE_AUDIO_CAPTURE, 0x2eef81be, 0x33fa, 0x4800, 0x96, 0x70, 0x1c, 0xd4, 0x74, 0x97, 0x2c, 0x3f);

// [uuid(2a59116d-6c4f-45e0-a74f-707e3fef9258)]
interface IAudioPolicyConfig : IInspectable
{
	virtual HRESULT STDMETHODCALLTYPE __incomplete__add_CtxVolumeChanged() = 0;
	virtual HRESULT STDMETHODCALLTYPE __incomplete__remove_CtxVolumeChanged() = 0;
	virtual HRESULT STDMETHODCALLTYPE __incomplete__add_RingerVibrateStateChanged() = 0;
	virtual HRESULT STDMETHODCALLTYPE __incomplete__remove_RingerVibrateStateChange() = 0;
	virtual HRESULT STDMETHODCALLTYPE __incomplete__SetVolumeGroupGainForId() = 0;
	virtual HRESULT STDMETHODCALLTYPE __incomplete__GetVolumeGroupGainForId() = 0;
	virtual HRESULT STDMETHODCALLTYPE __incomplete__GetActiveVolumeGroupForEndpointId() = 0;
	virtual HRESULT STDMETHODCALLTYPE __incomplete__GetVolumeGroupsForEndpoint() = 0;
	virtual HRESULT STDMETHODCALLTYPE __incomplete__GetCurrentVolumeContext() = 0;
	virtual HRESULT STDMETHODCALLTYPE __incomplete__SetVolumeGroupMuteForId() = 0;
	virtual HRESULT STDMETHODCALLTYPE __incomplete__GetVolumeGroupMuteForId() = 0;
	virtual HRESULT STDMETHODCALLTYPE __incomplete__SetRingerVibrateState() = 0;
	virtual HRESULT STDMETHODCALLTYPE __incomplete__GetRingerVibrateState() = 0;
	virtual HRESULT STDMETHODCALLTYPE __incomplete__SetPreferredChatApplication() = 0;
	virtual HRESULT STDMETHODCALLTYPE __incomplete__ResetPreferredChatApplication() = 0;
	virtual HRESULT STDMETHODCALLTYPE __incomplete__GetAvailableBlockCount() = 0;
	virtual HRESULT STDMETHODCALLTYPE __incomplete__GetCurrentChatApplications() = 0;
	virtual HRESULT STDMETHODCALLTYPE __incomplete__add_ChatContextChanged() = 0;
	virtual HRESULT STDMETHODCALLTYPE __incomplete__remove_ChatContextChanged() = 0;
	virtual HRESULT STDMETHODCALLTYPE SetPersistedDefaultAudioEndpoint(DWORD processId, EDataFlow flow, ERole role, HSTRING deviceId) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetPersistedDefaultAudioEndpoint(DWORD processId, EDataFlow flow, ERole role, HSTRING* deviceId) = 0;
	virtual HRESULT STDMETHODCALLTYPE ClearAllPersistedApplicationDefaultEndpoints() = 0;
};

// Provides access to functions in combase.dll which may not be available on
// Windows 7. Loads functions dynamically at runtime to prevent library
// dependencies.
class CombaseFunctions
{
public:
	CombaseFunctions() = default;
	~CombaseFunctions()
	{
		if (combase_dll_)
			FreeLibrary(combase_dll_);
	}

	bool LoadFunctions()
	{
		combase_dll_ = LoadLibraryW(L"combase.dll");
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

	HRESULT RoGetActivationFactory(HSTRING class_id, const IID& iid, void** out_factory)
	{
		return get_factory_func_(class_id, iid, out_factory);
	}

	HRESULT WindowsCreateString(const WCHAR* src, uint32_t len, HSTRING* out_hstr)
	{
		return create_string_func_(src, len, out_hstr);
	}

	HRESULT WindowsDeleteString(HSTRING hstr) {
		return delete_string_func_(hstr);
	}

	const WCHAR* WindowsGetStringRawBuffer(HSTRING hstr, uint32_t* out_len) {
		return get_string_raw_buffer_func_(hstr, out_len);
	}

	static CombaseFunctions* get()
	{
		static CombaseFunctions _;
		static bool initialized = false;
		if (!initialized)
			initialized = _.LoadFunctions();

		return &_;
	}

private:
	HMODULE combase_dll_ = nullptr;
	decltype(&::RoGetActivationFactory) get_factory_func_ = nullptr;
	decltype(&::WindowsCreateString) create_string_func_ = nullptr;
	decltype(&::WindowsDeleteString) delete_string_func_ = nullptr;
	decltype(&::WindowsGetStringRawBuffer) get_string_raw_buffer_func_ = nullptr;
};

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

static std::mutex s_audioEndpointLock;
static std::set<IDirectSoundBuffer*> s_soundBuffers;
static std::atomic<bool> s_threadCreated = false;
static bool s_endpointReset = false;

DWORD WINAPI ResetAppAudioEndpointThread(LPVOID lpParam)
{
	while (true)
	{
		{
			std::lock_guard<std::mutex> lock(s_audioEndpointLock);
			bool soundPlaying = false;
			for (auto itr = s_soundBuffers.begin(); itr != s_soundBuffers.end();)
			{
				auto soundBuffer = *itr;
				DWORD dwStatus;
				if (SUCCEEDED(soundBuffer->GetStatus(&dwStatus)))
				{
					if ((dwStatus & DSBSTATUS_BUFFERLOST)
						|| (dwStatus & DSBSTATUS_TERMINATED))
					{
						soundBuffer->Release();
						itr = s_soundBuffers.erase(itr);
						continue;
					}

					if (dwStatus & DSBSTATUS_PLAYING)
					{
						soundPlaying = true;
						break;
					}

					++itr;
				}
			}

			if (soundPlaying)
				break;
		}

		Sleep(100);
	}

	if (FAILED(CoInitialize(nullptr)))
	{
		WriteLog(L"ResetAppAudioEndpointThread(): CoInitialize() failed.\n");
		s_threadCreated = false;
		return 1;
	}

	ScopedHString classId(L"Windows.Media.Internal.AudioPolicyConfig");
	IAudioPolicyConfig* config = nullptr;
	if (FAILED(CombaseFunctions::get()->RoGetActivationFactory(classId.get(), IID_IAudioPolicyConfig, (void**)&config)))
	{
		WriteLog(L"ResetAppAudioEndpointThread(): RoGetActivationFactory() failed\n");
		CoUninitialize();
		s_threadCreated = false;
		return 2;
	}

	HSTRING deviceId_ = nullptr;
	HRESULT hr = config->GetPersistedDefaultAudioEndpoint(g_processId, eRender, eMultimedia, &deviceId_);
	if (FAILED(hr))
	{
		WriteLog(L"ResetAppAudioEndpointThread(): GetPersistedDefaultAudioEndpoint() failed (pid=%X). hr=%X\n", g_processId, hr);
		CoUninitialize();
		s_threadCreated = false;
		return 3;
	}

	ScopedHString deviceId(deviceId_);

	config->SetPersistedDefaultAudioEndpoint(g_processId, eRender, eMultimedia, deviceId.get());
	config->SetPersistedDefaultAudioEndpoint(g_processId, eRender, eConsole, deviceId.get());

	{
		std::lock_guard<std::mutex> lock(s_audioEndpointLock);
		for (auto itr = s_soundBuffers.begin(); itr != s_soundBuffers.end();)
		{
			(*itr)->Release();
			itr = s_soundBuffers.erase(itr);
		}

		s_endpointReset = true;
	}


	CoUninitialize();
	return 0;
}

void ResetAppAudioEndpoint(IDirectSoundBuffer* soundBuffer)
{
	std::lock_guard<std::mutex> lock(s_audioEndpointLock);
	if (s_endpointReset)
		return;

	bool expected = false;
	if (s_threadCreated.compare_exchange_weak(expected, true))
	{
		DWORD dwThreadId;
		if (CreateThread(nullptr, 0, ResetAppAudioEndpointThread, nullptr, 0, &dwThreadId) == nullptr)
			s_threadCreated = false;
	}

	if (s_soundBuffers.insert(soundBuffer).second)
		soundBuffer->AddRef();
}
