#include "stdafx.h"
#include "My_IDirectSound8.h"
#include "My_IDirectSoundBuffer8.h"

My_IDirectSound8::My_IDirectSound8(IDirectSound8* original)
	: m_original(original)
{
}

HRESULT My_IDirectSound8::QueryInterface(REFIID riid, LPVOID*  ppvObj)
{
	HRESULT hr = m_original->QueryInterface(riid, ppvObj);
	if (SUCCEEDED(hr))
		*ppvObj = this;

	return hr;
}

ULONG My_IDirectSound8::AddRef()
{
	return m_original->AddRef();
}

ULONG My_IDirectSound8::Release()
{
	ULONG ref = m_original->Release();
	if (ref == 0)
		delete this;

	return ref;
}

// IDirectSound methods
HRESULT My_IDirectSound8::CreateSoundBuffer(LPCDSBUFFERDESC pcDSBufferDesc, LPDIRECTSOUNDBUFFER* ppDSBuffer, LPUNKNOWN pUnkOuter)
{
	// NOTE: this should be handled with more nuance via cooperative levels to ensure actual compatibility,
	// and probably using another setting
	// This is more for sharing audio with other applications, not *strictly* supporting background audio.
#if 0
	if (g_settings.UseBackgroundAudio)
	{
		if (pcDSBufferDesc != nullptr
			&& pcDSBufferDesc->dwSize != 0)
		{
			uint8_t* bufferDesc = new uint8_t[pcDSBufferDesc->dwSize];
			memcpy(bufferDesc, pcDSBufferDesc, pcDSBufferDesc->dwSize);
			((LPDSBUFFERDESC)bufferDesc)->dwFlags |= DSBCAPS_GLOBALFOCUS;

			HRESULT hr = m_original->CreateSoundBuffer((LPCDSBUFFERDESC)bufferDesc, ppDSBuffer, pUnkOuter);

			if (bufferDesc != nullptr)
				delete[] bufferDesc;

			return hr;
}
	}
#endif

	HRESULT hr = m_original->CreateSoundBuffer(pcDSBufferDesc, ppDSBuffer, pUnkOuter);
	if (SUCCEEDED(hr))
		*ppDSBuffer = new My_IDirectSoundBuffer8(*ppDSBuffer);

	return hr;
}

HRESULT My_IDirectSound8::GetCaps(LPDSCAPS pDSCaps)
{
	return m_original->GetCaps(pDSCaps);
}

HRESULT My_IDirectSound8::DuplicateSoundBuffer(LPDIRECTSOUNDBUFFER pDSBufferOriginal, LPDIRECTSOUNDBUFFER* ppDSBufferDuplicate)
{
	HRESULT hr = m_original->DuplicateSoundBuffer(pDSBufferOriginal, ppDSBufferDuplicate);
	if (SUCCEEDED(hr))
		*ppDSBufferDuplicate = new My_IDirectSoundBuffer8(*ppDSBufferDuplicate);
	return hr;
}

HRESULT My_IDirectSound8::SetCooperativeLevel(HWND hwnd, DWORD dwLevel)
{
	return m_original->SetCooperativeLevel(hwnd, dwLevel);
}

HRESULT My_IDirectSound8::Compact()
{
	return m_original->Compact();
}

HRESULT My_IDirectSound8::GetSpeakerConfig(LPDWORD pdwSpeakerConfig)
{
	return m_original->GetSpeakerConfig(pdwSpeakerConfig);
}

HRESULT My_IDirectSound8::SetSpeakerConfig(DWORD dwSpeakerConfig)
{
	return m_original->SetSpeakerConfig(dwSpeakerConfig);
}

HRESULT My_IDirectSound8::Initialize(LPCGUID pcGuidDevice)
{
	return m_original->Initialize(pcGuidDevice);
}

// IDirectSound8 methods
HRESULT My_IDirectSound8::VerifyCertification(LPDWORD pdwCertified)
{
	return m_original->VerifyCertification(pdwCertified);
}
