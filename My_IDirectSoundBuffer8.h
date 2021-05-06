#pragma once

#include "My_IDirectSoundBuffer.h"

class My_IDirectSoundBuffer8 : public My_IDirectSoundBuffer
{
public:
	My_IDirectSoundBuffer8(IDirectSoundBuffer* original);

	STDMETHOD(SetFX)                (THIS_ DWORD dwEffectsCount, _In_reads_opt_(dwEffectsCount) LPDSEFFECTDESC pDSFXDesc, _Out_writes_opt_(dwEffectsCount) LPDWORD pdwResultCodes);
	STDMETHOD(AcquireResources)     (THIS_ DWORD dwFlags, DWORD dwEffectsCount, _Out_writes_(dwEffectsCount) LPDWORD pdwResultCodes);
	STDMETHOD(GetObjectInPath)      (THIS_ _In_ REFGUID rguidObject, DWORD dwIndex, _In_ REFGUID rguidInterface, _Outptr_ LPVOID* ppObject);
};
