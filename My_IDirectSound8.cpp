#include "stdafx.h"
#include "My_IDirectSound8.h"

My_IDirectSound8::My_IDirectSound8(IDirectSound8* original)
	: m_original(original)
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

	return m_original->QueryInterface(riid, ppvObj);
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

HRESULT My_IDirectSound8::GetCaps(LPDSCAPS pDSCaps)
{
	return m_original->GetCaps(pDSCaps);
}

HRESULT My_IDirectSound8::DuplicateSoundBuffer(LPDIRECTSOUNDBUFFER pDSBufferOriginal, LPDIRECTSOUNDBUFFER* ppDSBufferDuplicate)
{
	return m_original->DuplicateSoundBuffer(pDSBufferOriginal, ppDSBufferDuplicate);
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
