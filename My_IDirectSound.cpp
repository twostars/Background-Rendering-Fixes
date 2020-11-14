#include "stdafx.h"
#include "My_IDirectSound.h"

My_IDirectSound::My_IDirectSound(IDirectSound* original)
	: m_original(original)
{
}

HRESULT My_IDirectSound::QueryInterface(REFIID riid, LPVOID* ppvObj)
{
	if ((riid == IID_IDirectSound || riid == IID_IUnknown)
		&& ppvObj != nullptr)
	{
		AddRef();

		*ppvObj = this;
		return S_OK;
	}

	return m_original->QueryInterface(riid, ppvObj);
}

ULONG My_IDirectSound::AddRef()
{
	return m_original->AddRef();
}

ULONG My_IDirectSound::Release()
{
	ULONG ref = m_original->Release();
	if (ref == 0)
		delete this;

	return ref;
}

// IDirectSound methods
HRESULT My_IDirectSound::CreateSoundBuffer(LPCDSBUFFERDESC pcDSBufferDesc, LPDIRECTSOUNDBUFFER* ppDSBuffer, LPUNKNOWN pUnkOuter)
{
	void* bufferDesc = nullptr;
	if (pcDSBufferDesc != nullptr
		&& pcDSBufferDesc->dwSize != 0)
	{
		bufferDesc = new uint8_t[pcDSBufferDesc->dwSize];
		memcpy(bufferDesc, pcDSBufferDesc, pcDSBufferDesc->dwSize);
		((LPDSBUFFERDESC)bufferDesc)->dwFlags |= DSBCAPS_GLOBALFOCUS;
	}

	HRESULT hr = m_original->CreateSoundBuffer((LPCDSBUFFERDESC)bufferDesc, ppDSBuffer, pUnkOuter);

	if (bufferDesc != nullptr)
		delete bufferDesc;

	return hr;
}

HRESULT My_IDirectSound::GetCaps(LPDSCAPS pDSCaps)
{
	return m_original->GetCaps(pDSCaps);
}

HRESULT My_IDirectSound::DuplicateSoundBuffer(LPDIRECTSOUNDBUFFER pDSBufferOriginal, LPDIRECTSOUNDBUFFER* ppDSBufferDuplicate)
{
	return m_original->DuplicateSoundBuffer(pDSBufferOriginal, ppDSBufferDuplicate);
}

HRESULT My_IDirectSound::SetCooperativeLevel(HWND hwnd, DWORD dwLevel)
{
	return m_original->SetCooperativeLevel(hwnd, dwLevel);
}

HRESULT My_IDirectSound::Compact()
{
	return m_original->Compact();
}

HRESULT My_IDirectSound::GetSpeakerConfig(LPDWORD pdwSpeakerConfig)
{
	return m_original->GetSpeakerConfig(pdwSpeakerConfig);
}

HRESULT My_IDirectSound::SetSpeakerConfig(DWORD dwSpeakerConfig)
{
	return m_original->SetSpeakerConfig(dwSpeakerConfig);
}

HRESULT My_IDirectSound::Initialize(LPCGUID pcGuidDevice)
{
	return m_original->Initialize(pcGuidDevice);
}
