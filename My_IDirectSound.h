#pragma once

class My_IDirectSound : public IDirectSound
{
public:
	My_IDirectSound(IDirectSound* original);

	// IUnknown methods
	STDMETHOD(QueryInterface)(THIS_ _In_ REFIID, _Outptr_ LPVOID*);
	STDMETHOD_(ULONG, AddRef)(THIS);
	STDMETHOD_(ULONG, Release)(THIS);

	// IDirectSound methods
	STDMETHOD(CreateSoundBuffer)(THIS_ _In_ LPCDSBUFFERDESC pcDSBufferDesc, _Out_ LPDIRECTSOUNDBUFFER* ppDSBuffer, _Pre_null_ LPUNKNOWN pUnkOuter);
	STDMETHOD(GetCaps)(THIS_ _Out_ LPDSCAPS pDSCaps);
	STDMETHOD(DuplicateSoundBuffer)(THIS_ _In_ LPDIRECTSOUNDBUFFER pDSBufferOriginal, _Out_ LPDIRECTSOUNDBUFFER* ppDSBufferDuplicate);
	STDMETHOD(SetCooperativeLevel)(THIS_ HWND hwnd, DWORD dwLevel);
	STDMETHOD(Compact)(THIS);
	STDMETHOD(GetSpeakerConfig)(THIS_ _Out_ LPDWORD pdwSpeakerConfig);
	STDMETHOD(SetSpeakerConfig)(THIS_ DWORD dwSpeakerConfig);
	STDMETHOD(Initialize)(THIS_ _In_opt_ LPCGUID pcGuidDevice);

protected:
	IDirectSound* m_original;
};
