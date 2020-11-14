#pragma once

#include "IXAudio20.h"

class My_IXAudio20 : public IXAudio20
{
public:
	My_IXAudio20(IXAudio20* original);

	STDMETHOD(QueryInterface) (THIS_ REFIID riid, __deref_out void** ppvInterface) override;
	STDMETHOD_(ULONG, AddRef) (THIS) override;
	STDMETHOD_(ULONG, Release) (THIS) override;
	STDMETHOD(GetDeviceCount) (THIS_ __out UINT32* pCount) override;
	STDMETHOD(GetDeviceDetails) (THIS_ UINT32 Index, __out XAUDIO20_DEVICE_DETAILS* pDeviceDetails) override;
	STDMETHOD(Initialize) (THIS_ UINT32 Flags X20DEFAULT(0),
		XAUDIO20_PROCESSOR XAudio2Processor X20DEFAULT(XAUDIO20_DEFAULT_PROCESSOR)) override;
	STDMETHOD(RegisterForCallbacks) (__in IXAudio20EngineCallback* pCallback) override;
	STDMETHOD_(void, UnregisterForCallbacks) (__in IXAudio20EngineCallback* pCallback) override;

	STDMETHOD(CreateSourceVoice) (THIS_ __deref_out IXAudio20SourceVoice** ppSourceVoice,
		__in const WAVEFORMATEX* pSourceFormat,
		UINT32 Flags X20DEFAULT(0),
		float MaxFrequencyRatio X20DEFAULT(XAUDIO20_DEFAULT_FREQ_RATIO),
		__in_opt IXAudio20VoiceCallback* pCallback X20DEFAULT(NULL),
		__in_opt const XAUDIO20_VOICE_SENDS* pSendList X20DEFAULT(NULL),
		__in_opt const XAUDIO20_EFFECT_CHAIN* pEffectChain X20DEFAULT(NULL)) override;

	STDMETHOD(CreateSubmixVoice) (THIS_ __deref_out IXAudio20SubmixVoice** ppSubmixVoice,
		UINT32 InputChannels, UINT32 InputSampleRate,
		UINT32 Flags X20DEFAULT(0), UINT32 ProcessingStage X20DEFAULT(0),
		__in_opt const XAUDIO20_VOICE_SENDS* pSendList X20DEFAULT(NULL),
		__in_opt const XAUDIO20_EFFECT_CHAIN* pEffectChain X20DEFAULT(NULL)) override;

	STDMETHOD(CreateMasteringVoice) (THIS_ __deref_out IXAudio20MasteringVoice** ppMasteringVoice,
		UINT32 InputChannels X20DEFAULT(XAUDIO20_DEFAULT_CHANNELS),
		UINT32 InputSampleRate X20DEFAULT(XAUDIO20_DEFAULT_SAMPLERATE),
		UINT32 Flags X20DEFAULT(0), UINT32 DeviceIndex X20DEFAULT(0),
		__in_opt const XAUDIO20_EFFECT_CHAIN* pEffectChain X20DEFAULT(NULL)) override;

	STDMETHOD(StartEngine) (THIS) override;
	STDMETHOD_(void, StopEngine) (THIS) override;
	STDMETHOD(CommitChanges) (THIS_ UINT32 OperationSet) override;
	STDMETHOD_(void, GetPerformanceData) (THIS_ __out XAUDIO20_PERFORMANCE_DATA* pPerfData) override;
	STDMETHOD_(void, SetDebugConfiguration) (THIS_ __in_opt const XAUDIO20_DEBUG_CONFIGURATION* pDebugConfiguration,
		__in_opt __reserved void* pReserved X20DEFAULT(NULL)) override;

protected:
	IXAudio20* m_original;
};
