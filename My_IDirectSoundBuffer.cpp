#include "stdafx.h"
#include "My_IDirectSoundBuffer.h"
#include "reset_app_audio_endpoint.h"

My_IDirectSoundBuffer::My_IDirectSoundBuffer(IDirectSoundBuffer* original)
	: m_original(original)
{
}

HRESULT My_IDirectSoundBuffer::QueryInterface(REFIID riid, LPVOID* ppvObj)
{
	HRESULT hr = m_original->QueryInterface(riid, ppvObj);
	if (SUCCEEDED(hr))
		*ppvObj = this;

	return hr;
}

ULONG My_IDirectSoundBuffer::AddRef()
{
	return m_original->AddRef();
}

ULONG My_IDirectSoundBuffer::Release()
{
	ULONG ref = m_original->Release();
	if (ref == 0)
		delete this;

	return ref;
}

HRESULT My_IDirectSoundBuffer::GetCaps(_Out_ LPDSBCAPS pDSBufferCaps)
{
	return m_original->GetCaps(pDSBufferCaps);
}

HRESULT My_IDirectSoundBuffer::GetCurrentPosition(_Out_opt_ LPDWORD pdwCurrentPlayCursor, _Out_opt_ LPDWORD pdwCurrentWriteCursor)
{
	return m_original->GetCurrentPosition(pdwCurrentPlayCursor, pdwCurrentWriteCursor);
}

HRESULT My_IDirectSoundBuffer::GetFormat(_Out_writes_bytes_opt_(dwSizeAllocated) LPWAVEFORMATEX pwfxFormat, DWORD dwSizeAllocated, _Out_opt_ LPDWORD pdwSizeWritten)
{
	return m_original->GetFormat(pwfxFormat, dwSizeAllocated, pdwSizeWritten);
}

HRESULT My_IDirectSoundBuffer::GetVolume(_Out_ LPLONG plVolume)
{
	return m_original->GetVolume(plVolume);
}

HRESULT My_IDirectSoundBuffer::GetPan(_Out_ LPLONG plPan)
{
	return m_original->GetPan(plPan);
}

HRESULT My_IDirectSoundBuffer::GetFrequency(_Out_ LPDWORD pdwFrequency)
{
	return m_original->GetFrequency(pdwFrequency);
}

HRESULT My_IDirectSoundBuffer::GetStatus(_Out_ LPDWORD pdwStatus)
{
	return m_original->GetStatus(pdwStatus);
}

HRESULT My_IDirectSoundBuffer::Initialize(_In_ LPDIRECTSOUND pDirectSound, _In_ LPCDSBUFFERDESC pcDSBufferDesc)
{
	return m_original->Initialize(pDirectSound, pcDSBufferDesc);
}

HRESULT My_IDirectSoundBuffer::Lock(DWORD dwOffset, DWORD dwBytes,
	_Outptr_result_bytebuffer_(*pdwAudioBytes1) LPVOID* ppvAudioPtr1, _Out_ LPDWORD pdwAudioBytes1,
	_Outptr_opt_result_bytebuffer_(*pdwAudioBytes2) LPVOID* ppvAudioPtr2, _Out_opt_ LPDWORD pdwAudioBytes2, DWORD dwFlags)
{
	return m_original->Lock(dwOffset, dwBytes, ppvAudioPtr1, pdwAudioBytes1, ppvAudioPtr2, pdwAudioBytes2, dwFlags);
}

HRESULT My_IDirectSoundBuffer::Play(DWORD dwReserved1, DWORD dwPriority, DWORD dwFlags)
{
	HRESULT hr = m_original->Play(dwReserved1, dwPriority, dwFlags);
	if (SUCCEEDED(hr)
		&& g_settings.UseAppAudioDevice)
		ResetAppAudioEndpoint(m_original);

	return hr;
}

HRESULT My_IDirectSoundBuffer::SetCurrentPosition(DWORD dwNewPosition)
{
	return m_original->SetCurrentPosition(dwNewPosition);
}

HRESULT My_IDirectSoundBuffer::SetFormat(_In_ LPCWAVEFORMATEX pcfxFormat)
{
	return m_original->SetFormat(pcfxFormat);
}

HRESULT My_IDirectSoundBuffer::SetVolume(LONG lVolume)
{
	return m_original->SetVolume(lVolume);
}

HRESULT My_IDirectSoundBuffer::SetPan(LONG lPan)
{
	return m_original->SetPan(lPan);
}

HRESULT My_IDirectSoundBuffer::SetFrequency(DWORD dwFrequency)
{
	return m_original->SetFrequency(dwFrequency);
}

HRESULT My_IDirectSoundBuffer::Stop()
{
	return m_original->Stop();
}

HRESULT My_IDirectSoundBuffer::Unlock(
	_In_reads_bytes_(dwAudioBytes1) LPVOID pvAudioPtr1, DWORD dwAudioBytes1,
	_In_reads_bytes_opt_(dwAudioBytes2) LPVOID pvAudioPtr2, DWORD dwAudioBytes2)
{
	return m_original->Unlock(pvAudioPtr1, dwAudioBytes1, pvAudioPtr2, dwAudioBytes2);
}

HRESULT My_IDirectSoundBuffer::Restore()
{
	return m_original->Restore();
}
