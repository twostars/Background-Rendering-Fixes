#include "stdafx.h"
#include "My_IDirectSoundBuffer8.h"

My_IDirectSoundBuffer8::My_IDirectSoundBuffer8(IDirectSoundBuffer* original)
	: My_IDirectSoundBuffer(original)
{
}

HRESULT My_IDirectSoundBuffer8::QueryInterface(REFIID riid, LPVOID* ppvObj)
{
	HRESULT hr = m_original->QueryInterface(riid, ppvObj);
	if (SUCCEEDED(hr))
	{
		if (riid == IID_IDirectSoundBuffer
			|| riid == IID_IDirectSoundBuffer8)
			*ppvObj = this;
	}

	return hr;
}

HRESULT My_IDirectSoundBuffer8::SetFX(DWORD dwEffectsCount, _In_reads_opt_(dwEffectsCount) LPDSEFFECTDESC pDSFXDesc, _Out_writes_opt_(dwEffectsCount) LPDWORD pdwResultCodes)
{
	return static_cast<IDirectSoundBuffer8*>(m_original)->SetFX(dwEffectsCount, pDSFXDesc, pdwResultCodes);
}

HRESULT My_IDirectSoundBuffer8::AcquireResources(DWORD dwFlags, DWORD dwEffectsCount, _Out_writes_(dwEffectsCount) LPDWORD pdwResultCodes)
{
	return static_cast<IDirectSoundBuffer8*>(m_original)->AcquireResources(dwFlags, dwEffectsCount, pdwResultCodes);
}

HRESULT My_IDirectSoundBuffer8::GetObjectInPath(_In_ REFGUID rguidObject, DWORD dwIndex, _In_ REFGUID rguidInterface, _Outptr_ LPVOID* ppObject)
{
	return static_cast<IDirectSoundBuffer8*>(m_original)->GetObjectInPath(rguidObject, dwIndex, rguidInterface, ppObject);
}
