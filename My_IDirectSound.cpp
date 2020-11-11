#include "stdafx.h"
#include "My_IDirectSound.h"

My_IDirectSound::My_IDirectSound(LPDIRECTSOUND pSound)
	: m_proxy(pSound)
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

	return m_proxy->QueryInterface(riid, ppvObj);
}

ULONG My_IDirectSound::AddRef()
{
	return m_proxy->AddRef();
}

ULONG My_IDirectSound::Release()
{
	ULONG ref = m_proxy->Release();

	if (ref == 0)
	{
		delete this;
	}

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

	HRESULT hr = m_proxy->CreateSoundBuffer((LPCDSBUFFERDESC)bufferDesc, ppDSBuffer, pUnkOuter);

	if (bufferDesc != nullptr)
		delete bufferDesc;

	return hr;
}

HRESULT My_IDirectSound::GetCaps(LPDSCAPS pDSCaps)
{
	return m_proxy->GetCaps(pDSCaps);
}

HRESULT My_IDirectSound::DuplicateSoundBuffer(LPDIRECTSOUNDBUFFER pDSBufferOriginal, LPDIRECTSOUNDBUFFER* ppDSBufferDuplicate)
{
	return m_proxy->DuplicateSoundBuffer(pDSBufferOriginal, ppDSBufferDuplicate);
}

HRESULT My_IDirectSound::SetCooperativeLevel(HWND hwnd, DWORD dwLevel)
{
	return m_proxy->SetCooperativeLevel(hwnd, dwLevel);
}

HRESULT My_IDirectSound::Compact()
{
	return m_proxy->Compact();
}

HRESULT My_IDirectSound::GetSpeakerConfig(LPDWORD pdwSpeakerConfig)
{
	return m_proxy->GetSpeakerConfig(pdwSpeakerConfig);
}

HRESULT My_IDirectSound::SetSpeakerConfig(DWORD dwSpeakerConfig)
{
	return m_proxy->SetSpeakerConfig(dwSpeakerConfig);
}

HRESULT My_IDirectSound::Initialize(LPCGUID pcGuidDevice)
{
	return m_proxy->Initialize(pcGuidDevice);
}
