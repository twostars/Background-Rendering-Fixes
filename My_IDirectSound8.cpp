#include "stdafx.h"
#include "My_IDirectSound8.h"

My_IDirectSound8::My_IDirectSound8(LPDIRECTSOUND8 pSound)
	: m_proxy(pSound)
{
}

HRESULT My_IDirectSound8::QueryInterface(REFIID riid, LPVOID*  ppvObj)
{
	if ((riid == IID_IDirectSound || riid == IID_IDirectSound8 || riid == IID_IUnknown)
		&& ppvObj != nullptr)
	{
		AddRef();

		*ppvObj = this;
		return S_OK;
	}

	return m_proxy->QueryInterface(riid, ppvObj);
}

ULONG My_IDirectSound8::AddRef()
{
	return m_proxy->AddRef();
}

ULONG My_IDirectSound8::Release()
{
	ULONG ref = m_proxy->Release();
	if (ref == 0)
		delete this;

	return ref;
}

// IDirectSound methods
HRESULT My_IDirectSound8::CreateSoundBuffer(LPCDSBUFFERDESC pcDSBufferDesc, LPDIRECTSOUNDBUFFER* ppDSBuffer, LPUNKNOWN pUnkOuter)
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

HRESULT My_IDirectSound8::GetCaps(LPDSCAPS pDSCaps)
{
	return m_proxy->GetCaps(pDSCaps);
}

HRESULT My_IDirectSound8::DuplicateSoundBuffer(LPDIRECTSOUNDBUFFER pDSBufferOriginal, LPDIRECTSOUNDBUFFER* ppDSBufferDuplicate)
{
	return m_proxy->DuplicateSoundBuffer(pDSBufferOriginal, ppDSBufferDuplicate);
}

HRESULT My_IDirectSound8::SetCooperativeLevel(HWND hwnd, DWORD dwLevel)
{
	return m_proxy->SetCooperativeLevel(hwnd, dwLevel);
}

HRESULT My_IDirectSound8::Compact()
{
	return m_proxy->Compact();
}

HRESULT My_IDirectSound8::GetSpeakerConfig(LPDWORD pdwSpeakerConfig)
{
	return m_proxy->GetSpeakerConfig(pdwSpeakerConfig);
}

HRESULT My_IDirectSound8::SetSpeakerConfig(DWORD dwSpeakerConfig)
{
	return m_proxy->SetSpeakerConfig(dwSpeakerConfig);
}

HRESULT My_IDirectSound8::Initialize(LPCGUID pcGuidDevice)
{
	return m_proxy->Initialize(pcGuidDevice);
}

// IDirectSound8 methods
HRESULT My_IDirectSound8::VerifyCertification(LPDWORD pdwCertified)
{
	return m_proxy->VerifyCertification(pdwCertified);
}
