#include "stdafx.h"

#include "CombaseFunctions.h"
#include "ScopedHString.h"

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

static std::mutex s_audioEndpointLock;
static std::set<IDirectSoundBuffer*> s_soundBuffers;
static std::atomic<bool> s_threadCreated = false;
static bool s_endpointReset = false;

static bool CheckAnySoundBufferIsPlaying()
{
	std::lock_guard<std::mutex> lock(s_audioEndpointLock);
	for (auto itr = s_soundBuffers.begin(); itr != s_soundBuffers.end();)
	{
		auto soundBuffer = *itr;
		DWORD dwStatus;
		if (FAILED(soundBuffer->GetStatus(&dwStatus)))
		{
			soundBuffer->Release();
			itr = s_soundBuffers.erase(itr);
			continue;
		}

		if ((dwStatus & DSBSTATUS_BUFFERLOST)
			|| (dwStatus & DSBSTATUS_TERMINATED))
		{
			soundBuffer->Release();
			itr = s_soundBuffers.erase(itr);
			continue;
		}

		if (dwStatus & DSBSTATUS_PLAYING)
			return true;

		++itr;
	}

	return false;
}

DWORD WINAPI ResetAppAudioEndpointThread(LPVOID lpParam)
{
	while (!CheckAnySoundBufferIsPlaying())
		Sleep(100);

	if (FAILED(CoInitialize(nullptr)))
	{
		if (g_settings.LogLevel <= LOGLEVEL_CRITICAL)
			WriteLog(L"ResetAppAudioEndpointThread(): CoInitialize() failed.\n");

		s_threadCreated = false;
		return 1;
	}

	ScopedHString classId(L"Windows.Media.Internal.AudioPolicyConfig");
	IAudioPolicyConfig* config = nullptr;
	if (FAILED(CombaseFunctions::get()->RoGetActivationFactory(classId.get(), IID_IAudioPolicyConfig, (void**)&config)))
	{
		if (g_settings.LogLevel <= LOGLEVEL_CRITICAL)
			WriteLog(L"ResetAppAudioEndpointThread(): RoGetActivationFactory() failed\n");

		CoUninitialize();
		s_threadCreated = false;
		return 2;
	}

	HSTRING deviceId_ = nullptr;
	HRESULT hr = config->GetPersistedDefaultAudioEndpoint(g_processId, eRender, eMultimedia, &deviceId_);
	if (FAILED(hr))
	{
		if (g_settings.LogLevel <= LOGLEVEL_CRITICAL)
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
