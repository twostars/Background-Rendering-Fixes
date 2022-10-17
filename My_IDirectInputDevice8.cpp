#include "stdafx.h"
#include "My_IDirectInputDevice8.h"
#include "My_IDirectInput8.h"

My_IDirectInputDevice8A::My_IDirectInputDevice8A(REFGUID guid, IDirectInputDevice8A* original, My_IDirectInput8A* di)
	: m_guid(guid), m_original(original), m_di(di)
{
	if (m_guid == GUID_SysMouse
		|| m_guid == GUID_SysMouseEm
		|| m_guid == GUID_SysMouseEm2)
		m_deviceType = INPUT_DEVICE_MOUSE;
	else if (m_guid == GUID_SysKeyboard
		|| m_guid == GUID_SysKeyboardEm
		|| m_guid == GUID_SysKeyboardEm2)
		m_deviceType = INPUT_DEVICE_KEYBOARD;
	else
		m_deviceType = INPUT_DEVICE_UNKNOWN;
}

HRESULT My_IDirectInputDevice8A::QueryInterface(REFIID riid, LPVOID* ppvObj)
{
	HRESULT hr = m_original->QueryInterface(riid, ppvObj);
	if (SUCCEEDED(hr))
		*ppvObj = this;

	return hr;
}

ULONG My_IDirectInputDevice8A::AddRef()
{
	return m_original->AddRef();
}

ULONG My_IDirectInputDevice8A::Release()
{
	ULONG ref = m_original->Release();
	if (ref == 0)
	{
		m_di->DetachDevice(this);
		delete this;
	}

	return ref;
}

HRESULT My_IDirectInputDevice8A::GetCapabilities(LPDIDEVCAPS lpDIDevCaps)
{
	return m_original->GetCapabilities(lpDIDevCaps);
}

HRESULT My_IDirectInputDevice8A::EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACKA lpCallback, LPVOID pvRef, DWORD dwFlags)
{
	return m_original->EnumObjects(lpCallback, pvRef, dwFlags);
}

HRESULT My_IDirectInputDevice8A::GetProperty(REFGUID rguidProp, LPDIPROPHEADER pdiph)
{
	return m_original->GetProperty(rguidProp, pdiph);
}

HRESULT My_IDirectInputDevice8A::SetProperty(REFGUID rguidProp, LPCDIPROPHEADER pdiph)
{
	return m_original->SetProperty(rguidProp, pdiph);
}

HRESULT My_IDirectInputDevice8A::Acquire()
{
	return m_original->Acquire();
}

HRESULT My_IDirectInputDevice8A::Unacquire()
{
	return m_original->Unacquire();
}

HRESULT My_IDirectInputDevice8A::GetDeviceState(DWORD cbData, LPVOID lpvData)
{
	return m_original->GetDeviceState(cbData, lpvData);
}

HRESULT My_IDirectInputDevice8A::GetDeviceData(DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags)
{
	HRESULT hr = m_original->GetDeviceData(cbObjectData, rgdod, pdwInOut, dwFlags);
	if (SUCCEEDED(hr))
	{
		if ((g_settings.UseBackgroundRendering && !g_applicationInFocus)
			|| (g_settings.DisableKeyboard && m_deviceType == INPUT_DEVICE_KEYBOARD)
			|| (g_settings.DisableMouse && m_deviceType == INPUT_DEVICE_MOUSE))
		{
			if (m_deviceType == INPUT_DEVICE_KEYBOARD
				|| m_deviceType == INPUT_DEVICE_MOUSE)
				memset(rgdod, 0, cbObjectData);
		}
	}

	return hr;
}

HRESULT My_IDirectInputDevice8A::SetDataFormat(LPCDIDATAFORMAT lpdf)
{
	return m_original->SetDataFormat(lpdf);
}

HRESULT My_IDirectInputDevice8A::SetEventNotification(HANDLE hEvent)
{
	return m_original->SetEventNotification(hEvent);
}

HRESULT My_IDirectInputDevice8A::SetCooperativeLevel(HWND hwnd, DWORD dwFlags)
{
	return m_original->SetCooperativeLevel(hwnd, dwFlags);
}

HRESULT My_IDirectInputDevice8A::GetObjectInfo(LPDIDEVICEOBJECTINSTANCEA pdidoi, DWORD dwObj, DWORD dwHow)
{
	return m_original->GetObjectInfo(pdidoi, dwObj, dwHow);
}

HRESULT My_IDirectInputDevice8A::GetDeviceInfo(LPDIDEVICEINSTANCEA pdidi)
{
	return m_original->GetDeviceInfo(pdidi);
}

HRESULT My_IDirectInputDevice8A::RunControlPanel(HWND hwndOwner, DWORD dwFlags)
{
	return m_original->RunControlPanel(hwndOwner, dwFlags);
}

HRESULT My_IDirectInputDevice8A::Initialize(HINSTANCE hinst, DWORD dwVersion, REFGUID rguid)
{
	return m_original->Initialize(hinst, dwVersion, rguid);
}

HRESULT My_IDirectInputDevice8A::CreateEffect(REFGUID rguid, LPCDIEFFECT lpeff, LPDIRECTINPUTEFFECT* ppdeff, LPUNKNOWN punkOuter)
{
	return m_original->CreateEffect(rguid, lpeff, ppdeff, punkOuter);
}

HRESULT My_IDirectInputDevice8A::EnumEffects(LPDIENUMEFFECTSCALLBACKA lpCallback, LPVOID pvRef, DWORD dwEffType)
{
	return m_original->EnumEffects(lpCallback, pvRef, dwEffType);
}

HRESULT My_IDirectInputDevice8A::GetEffectInfo(LPDIEFFECTINFOA pdei, REFGUID rguid)
{
	return m_original->GetEffectInfo(pdei, rguid);
}

HRESULT My_IDirectInputDevice8A::GetForceFeedbackState(LPDWORD pdwOut)
{
	return m_original->GetForceFeedbackState(pdwOut);
}

HRESULT My_IDirectInputDevice8A::SendForceFeedbackCommand(DWORD dwFlags)
{
	return m_original->SendForceFeedbackCommand(dwFlags);
}

HRESULT My_IDirectInputDevice8A::EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK lpCallback, LPVOID pvRef, DWORD fl)
{
	return m_original->EnumCreatedEffectObjects(lpCallback, pvRef, fl);
}

HRESULT My_IDirectInputDevice8A::Escape(LPDIEFFESCAPE pesc)
{
	return m_original->Escape(pesc);
}

HRESULT My_IDirectInputDevice8A::Poll()
{
	return m_original->Poll();
}

HRESULT My_IDirectInputDevice8A::SendDeviceData(DWORD cbObjectData, LPCDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD fl)
{
	return m_original->SendDeviceData(cbObjectData, rgdod, pdwInOut, fl);
}

HRESULT My_IDirectInputDevice8A::EnumEffectsInFile(LPCSTR lpszFileName, LPDIENUMEFFECTSINFILECALLBACK pec, LPVOID pvRef, DWORD dwFlags)
{
	return m_original->EnumEffectsInFile(lpszFileName, pec, pvRef, dwFlags);
}

HRESULT My_IDirectInputDevice8A::WriteEffectToFile(LPCSTR lpszFileName, DWORD dwEntries, LPDIFILEEFFECT rgDiFileEft, DWORD dwFlags)
{
	return m_original->WriteEffectToFile(lpszFileName, dwEntries, rgDiFileEft, dwFlags);
}

HRESULT My_IDirectInputDevice8A::BuildActionMap(LPDIACTIONFORMATA lpdiaf, LPCSTR lpszUserName, DWORD dwFlags)
{
	return m_original->BuildActionMap(lpdiaf, lpszUserName, dwFlags);
}

HRESULT My_IDirectInputDevice8A::SetActionMap(LPDIACTIONFORMATA lpdiActionFormat, LPCSTR lptszUserName, DWORD dwFlags)
{
	return m_original->SetActionMap(lpdiActionFormat, lptszUserName, dwFlags);
}

HRESULT My_IDirectInputDevice8A::GetImageInfo(LPDIDEVICEIMAGEINFOHEADERA lpdiDevImageInfoHeader)
{
	return m_original->GetImageInfo(lpdiDevImageInfoHeader);
}

My_IDirectInputDevice8W::My_IDirectInputDevice8W(REFGUID guid, IDirectInputDevice8W* original, My_IDirectInput8W* di)
	: m_guid(guid), m_original(original), m_di(di)
{
	if (m_guid == GUID_SysMouse
		|| m_guid == GUID_SysMouseEm
		|| m_guid == GUID_SysMouseEm2)
		m_deviceType = INPUT_DEVICE_MOUSE;
	else if (m_guid == GUID_SysKeyboard
		|| m_guid == GUID_SysKeyboardEm
		|| m_guid == GUID_SysKeyboardEm2)
		m_deviceType = INPUT_DEVICE_KEYBOARD;
	else
		m_deviceType = INPUT_DEVICE_UNKNOWN;
}

HRESULT My_IDirectInputDevice8W::QueryInterface(REFIID riid, LPVOID* ppvObj)
{
	HRESULT hr = m_original->QueryInterface(riid, ppvObj);
	if (SUCCEEDED(hr))
		*ppvObj = this;

	return hr;
}

ULONG My_IDirectInputDevice8W::AddRef()
{
	return m_original->AddRef();
}

ULONG My_IDirectInputDevice8W::Release()
{
	ULONG ref = m_original->Release();
	if (ref == 0)
	{
		m_di->DetachDevice(this);
		delete this;
	}

	return ref;
}

HRESULT My_IDirectInputDevice8W::GetCapabilities(LPDIDEVCAPS lpDIDevCaps)
{
	return m_original->GetCapabilities(lpDIDevCaps);
}

HRESULT My_IDirectInputDevice8W::EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACKW lpCallback, LPVOID pvRef, DWORD dwFlags)
{
	return m_original->EnumObjects(lpCallback, pvRef, dwFlags);
}

HRESULT My_IDirectInputDevice8W::GetProperty(REFGUID rguidProp, LPDIPROPHEADER pdiph)
{
	return m_original->GetProperty(rguidProp, pdiph);
}

HRESULT My_IDirectInputDevice8W::SetProperty(REFGUID rguidProp, LPCDIPROPHEADER pdiph)
{
	return m_original->SetProperty(rguidProp, pdiph);
}

HRESULT My_IDirectInputDevice8W::Acquire()
{
	return m_original->Acquire();
}

HRESULT My_IDirectInputDevice8W::Unacquire()
{
	return m_original->Unacquire();
}

HRESULT My_IDirectInputDevice8W::GetDeviceState(DWORD cbData, LPVOID lpvData)
{
	return m_original->GetDeviceState(cbData, lpvData);
}

HRESULT My_IDirectInputDevice8W::GetDeviceData(DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags)
{
	HRESULT hr = m_original->GetDeviceData(cbObjectData, rgdod, pdwInOut, dwFlags);
	if (SUCCEEDED(hr))
	{
		if ((g_settings.UseBackgroundRendering && !g_applicationInFocus)
			|| (g_settings.DisableKeyboard && m_deviceType == INPUT_DEVICE_KEYBOARD)
			|| (g_settings.DisableMouse && m_deviceType == INPUT_DEVICE_MOUSE))
		{
			if (m_deviceType == INPUT_DEVICE_KEYBOARD
				|| m_deviceType == INPUT_DEVICE_MOUSE)
				memset(rgdod, 0, cbObjectData);
		}
	}

	return hr;
}

HRESULT My_IDirectInputDevice8W::SetDataFormat(LPCDIDATAFORMAT lpdf)
{
	return m_original->SetDataFormat(lpdf);
}

HRESULT My_IDirectInputDevice8W::SetEventNotification(HANDLE hEvent)
{
	return m_original->SetEventNotification(hEvent);
}

HRESULT My_IDirectInputDevice8W::SetCooperativeLevel(HWND hwnd, DWORD dwFlags)
{
	return m_original->SetCooperativeLevel(hwnd, dwFlags);
}

HRESULT My_IDirectInputDevice8W::GetObjectInfo(LPDIDEVICEOBJECTINSTANCEW pdidoi, DWORD dwObj, DWORD dwHow)
{
	return m_original->GetObjectInfo(pdidoi, dwObj, dwHow);
}

HRESULT My_IDirectInputDevice8W::GetDeviceInfo(LPDIDEVICEINSTANCEW pdidi)
{
	return m_original->GetDeviceInfo(pdidi);
}

HRESULT My_IDirectInputDevice8W::RunControlPanel(HWND hwndOwner, DWORD dwFlags)
{
	return m_original->RunControlPanel(hwndOwner, dwFlags);
}

HRESULT My_IDirectInputDevice8W::Initialize(HINSTANCE hinst, DWORD dwVersion, REFGUID rguid)
{
	return m_original->Initialize(hinst, dwVersion, rguid);
}

HRESULT My_IDirectInputDevice8W::CreateEffect(REFGUID rguid, LPCDIEFFECT lpeff, LPDIRECTINPUTEFFECT* ppdeff, LPUNKNOWN punkOuter)
{
	return m_original->CreateEffect(rguid, lpeff, ppdeff, punkOuter);
}

HRESULT My_IDirectInputDevice8W::EnumEffects(LPDIENUMEFFECTSCALLBACKW lpCallback, LPVOID pvRef, DWORD dwEffType)
{
	return m_original->EnumEffects(lpCallback, pvRef, dwEffType);
}

HRESULT My_IDirectInputDevice8W::GetEffectInfo(LPDIEFFECTINFOW pdei, REFGUID rguid)
{
	return m_original->GetEffectInfo(pdei, rguid);
}

HRESULT My_IDirectInputDevice8W::GetForceFeedbackState(LPDWORD pdwOut)
{
	return m_original->GetForceFeedbackState(pdwOut);
}

HRESULT My_IDirectInputDevice8W::SendForceFeedbackCommand(DWORD dwFlags)
{
	return m_original->SendForceFeedbackCommand(dwFlags);
}

HRESULT My_IDirectInputDevice8W::EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK lpCallback, LPVOID pvRef, DWORD fl)
{
	return m_original->EnumCreatedEffectObjects(lpCallback, pvRef, fl);
}

HRESULT My_IDirectInputDevice8W::Escape(LPDIEFFESCAPE pesc)
{
	return m_original->Escape(pesc);
}

HRESULT My_IDirectInputDevice8W::Poll()
{
	return m_original->Poll();
}

HRESULT My_IDirectInputDevice8W::SendDeviceData(DWORD cbObjectData, LPCDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD fl)
{
	return m_original->SendDeviceData(cbObjectData, rgdod, pdwInOut, fl);
}

HRESULT My_IDirectInputDevice8W::EnumEffectsInFile(LPCWSTR lpszFileName, LPDIENUMEFFECTSINFILECALLBACK pec, LPVOID pvRef, DWORD dwFlags)
{
	return m_original->EnumEffectsInFile(lpszFileName, pec, pvRef, dwFlags);
}

HRESULT My_IDirectInputDevice8W::WriteEffectToFile(LPCWSTR lpszFileName, DWORD dwEntries, LPDIFILEEFFECT rgDiFileEft, DWORD dwFlags)
{
	return m_original->WriteEffectToFile(lpszFileName, dwEntries, rgDiFileEft, dwFlags);
}

HRESULT My_IDirectInputDevice8W::BuildActionMap(LPDIACTIONFORMATW lpdiaf, LPCWSTR lpszUserName, DWORD dwFlags)
{
	return m_original->BuildActionMap(lpdiaf, lpszUserName, dwFlags);
}

HRESULT My_IDirectInputDevice8W::SetActionMap(LPDIACTIONFORMATW lpdiActionFormat, LPCWSTR lptszUserName, DWORD dwFlags)
{
	return m_original->SetActionMap(lpdiActionFormat, lptszUserName, dwFlags);
}

HRESULT My_IDirectInputDevice8W::GetImageInfo(LPDIDEVICEIMAGEINFOHEADERW lpdiDevImageInfoHeader)
{
	return m_original->GetImageInfo(lpdiDevImageInfoHeader);
}
