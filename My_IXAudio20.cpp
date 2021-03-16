#include "stdafx.h"
#include "My_IXAudio20.h"
#include "utils.h"

My_IXAudio20::My_IXAudio20(IXAudio20* original)
	: m_original(original)
{
}

HRESULT My_IXAudio20::QueryInterface(REFIID riid, LPVOID* ppvObj)
{
	if ((riid == IID_IXAudio2 || riid == IID_IUnknown)
		&& ppvObj != nullptr)
	{
		AddRef();

		*ppvObj = this;
		return S_OK;
	}

	return m_original->QueryInterface(riid, ppvObj);
}

ULONG My_IXAudio20::AddRef()
{
	return m_original->AddRef();
}

ULONG My_IXAudio20::Release()
{
	ULONG ref = m_original->Release();
	if (ref == 0)
		delete this;

	return ref;
}

HRESULT My_IXAudio20::GetDeviceCount(UINT32* pCount)
{
	return m_original->GetDeviceCount(pCount);
}

HRESULT My_IXAudio20::GetDeviceDetails(UINT32 Index, XAUDIO20_DEVICE_DETAILS* pDeviceDetails)
{
	return m_original->GetDeviceDetails(Index, pDeviceDetails);
}

HRESULT My_IXAudio20::Initialize(UINT32 Flags, XAUDIO20_PROCESSOR XAudio2Processor)
{
	return m_original->Initialize(Flags, XAudio2Processor);
}

HRESULT My_IXAudio20::RegisterForCallbacks(IXAudio20EngineCallback* pCallback)
{
	return m_original->RegisterForCallbacks(pCallback);
}

void My_IXAudio20::UnregisterForCallbacks(IXAudio20EngineCallback* pCallback)
{
	m_original->UnregisterForCallbacks(pCallback);
}

HRESULT My_IXAudio20::CreateSourceVoice(
	IXAudio20SourceVoice** ppSourceVoice,
	const WAVEFORMATEX* pSourceFormat,
	UINT32 Flags,
	float MaxFrequencyRatio,
	IXAudio20VoiceCallback* pCallback,
	const XAUDIO20_VOICE_SENDS* pSendList,
	const XAUDIO20_EFFECT_CHAIN* pEffectChain)
{
	return m_original->CreateSourceVoice(
		ppSourceVoice,
		pSourceFormat,
		Flags,
		MaxFrequencyRatio,
		pCallback,
		pSendList,
		pEffectChain);
}

HRESULT My_IXAudio20::CreateSubmixVoice(
	IXAudio20SubmixVoice** ppSubmixVoice,
	UINT32 InputChannels,
	UINT32 InputSampleRate,
	UINT32 Flags,
	UINT32 ProcessingStage,
	const XAUDIO20_VOICE_SENDS* pSendList,
	const XAUDIO20_EFFECT_CHAIN* pEffectChain)
{
	return m_original->CreateSubmixVoice(
		ppSubmixVoice,
		InputChannels,
		InputSampleRate,
		Flags,
		ProcessingStage,
		pSendList,
		pEffectChain);
}

HRESULT My_IXAudio20::CreateMasteringVoice(
	IXAudio20MasteringVoice** ppMasteringVoice,
	UINT32 InputChannels,
	UINT32 InputSampleRate,
	UINT32 Flags,
	UINT32 DeviceIndex,
	const XAUDIO20_EFFECT_CHAIN* pEffectChain)
{
	// When the default is supplied, we should attempt to find the real default.
	// It is, however, unfortunate if we explicitly want to use device 0 over a default.
	if (g_settings.UseAppAudioDevice)
	{
		std::wstring deviceID;
		if (DeviceIndex == 0
			&& GetDefaultAudioEndpointIDForThisProcess(&deviceID))
		{
			UINT32 count = 0;
			if (SUCCEEDED(m_original->GetDeviceCount(&count)))
			{
				for (UINT32 i = 0; i < count; ++i)
				{
					XAUDIO20_DEVICE_DETAILS deviceDetails = { 0 };
					if (FAILED(m_original->GetDeviceDetails(i, &deviceDetails)))
						continue;

					if (wcsicmp(deviceID.c_str(), deviceDetails.DeviceID) == 0)
					{
						DeviceIndex = i;
						break;
					}
				}
			}
		}
	}

	return m_original->CreateMasteringVoice(
		ppMasteringVoice,
		InputChannels,
		InputSampleRate,
		Flags,
		DeviceIndex,
		pEffectChain);
}

HRESULT My_IXAudio20::StartEngine()
{
	return m_original->StartEngine();
}

void My_IXAudio20::StopEngine()
{
	m_original->StopEngine();
}

HRESULT My_IXAudio20::CommitChanges(UINT32 OperationSet)
{
	return m_original->CommitChanges(OperationSet);
}

void My_IXAudio20::GetPerformanceData(XAUDIO20_PERFORMANCE_DATA* pPerfData)
{
	return m_original->GetPerformanceData(pPerfData);
}

void My_IXAudio20::SetDebugConfiguration(
	const XAUDIO20_DEBUG_CONFIGURATION* pDebugConfiguration,
	void* pReserved)
{
	return m_original->SetDebugConfiguration(pDebugConfiguration, pReserved);
}
