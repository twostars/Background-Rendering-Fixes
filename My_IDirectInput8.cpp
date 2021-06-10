#include "stdafx.h"
#include "My_IDirectInput8.h"
#include "My_IDirectInputDevice8.h"

My_IDirectInput8A::My_IDirectInput8A(IDirectInput8A* original)
	: m_original(original)
{
}

HRESULT My_IDirectInput8A::QueryInterface(REFIID riid, LPVOID* ppvObj)
{
	HRESULT hr = m_original->QueryInterface(riid, ppvObj);
	if (SUCCEEDED(hr))
		*ppvObj = this;

	return hr;
}

ULONG My_IDirectInput8A::AddRef()
{
	return m_original->AddRef();
}

ULONG My_IDirectInput8A::Release()
{
	ULONG ref = m_original->Release();
	if (ref == 0)
		delete this;

	return ref;
}

HRESULT My_IDirectInput8A::CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICE8A* lplpDirectInputDevice, LPUNKNOWN pUnkOuter)
{
	if (g_settings.DisableMouse)
	{
		if (rguid == GUID_SysMouse
			|| rguid == GUID_SysMouseEm
			|| rguid == GUID_SysMouseEm2)
			return E_FAIL;
	}

	if (g_settings.DisableKeyboard)
	{
		if (rguid == GUID_SysKeyboard
			|| rguid == GUID_SysKeyboardEm
			|| rguid == GUID_SysKeyboardEm2)
			return E_FAIL;
	}

	HRESULT hr = m_original->CreateDevice(rguid, lplpDirectInputDevice, pUnkOuter);
	if (SUCCEEDED(hr))
	{
		auto device = new My_IDirectInputDevice8A(rguid, *lplpDirectInputDevice, this);
		*lplpDirectInputDevice = device;

		std::lock_guard<std::recursive_mutex> lock(m_lock);
		m_devices.insert(device);
	}

	return hr;
}

HRESULT My_IDirectInput8A::EnumDevices(DWORD dwDevType, LPDIENUMDEVICESCALLBACKA lpCallback, LPVOID pvRef, DWORD dwFlags)
{
	return m_original->EnumDevices(dwDevType, lpCallback, pvRef, dwFlags);
}

HRESULT My_IDirectInput8A::GetDeviceStatus(REFGUID rguidInstance)
{
	return m_original->GetDeviceStatus(rguidInstance);
}

HRESULT My_IDirectInput8A::RunControlPanel(HWND hwndOwner, DWORD dwFlags)
{
	return m_original->RunControlPanel(hwndOwner, dwFlags);
}

HRESULT My_IDirectInput8A::Initialize(HINSTANCE hinst, DWORD dwVersion)
{
	return m_original->Initialize(hinst, dwVersion);
}

HRESULT My_IDirectInput8A::FindDevice(REFGUID rguidClass, LPCSTR ptszName, LPGUID pguidInstance)
{
	return m_original->FindDevice(rguidClass, ptszName, pguidInstance);
}

HRESULT My_IDirectInput8A::EnumDevicesBySemantics(LPCSTR ptszUserName, LPDIACTIONFORMATA lpdiActionFormat, LPDIENUMDEVICESBYSEMANTICSCBA lpCallback, LPVOID pvRef, DWORD dwFlags)
{
	return m_original->EnumDevicesBySemantics(ptszUserName, lpdiActionFormat, lpCallback, pvRef, dwFlags);
}

HRESULT My_IDirectInput8A::ConfigureDevices(LPDICONFIGUREDEVICESCALLBACK lpdiCallback, LPDICONFIGUREDEVICESPARAMSA lpdiCDParams, DWORD dwFlags, LPVOID pvRefData)
{
	return m_original->ConfigureDevices(lpdiCallback, lpdiCDParams, dwFlags, pvRefData);
}

void My_IDirectInput8A::DetachDevice(My_IDirectInputDevice8A* device)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);
	m_devices.erase(device);
}

My_IDirectInput8W::My_IDirectInput8W(IDirectInput8W* original)
	: m_original(original)
{
}

HRESULT My_IDirectInput8W::QueryInterface(REFIID riid, LPVOID* ppvObj)
{
	HRESULT hr = m_original->QueryInterface(riid, ppvObj);
	if (SUCCEEDED(hr))
		*ppvObj = this;

	return hr;
}

ULONG My_IDirectInput8W::AddRef()
{
	return m_original->AddRef();
}

ULONG My_IDirectInput8W::Release()
{
	ULONG ref = m_original->Release();
	if (ref == 0)
		delete this;

	return ref;
}

HRESULT My_IDirectInput8W::CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICE8W* lplpDirectInputDevice, LPUNKNOWN pUnkOuter)
{
	if (g_settings.DisableMouse)
	{
		if (rguid == GUID_SysMouse
			|| rguid == GUID_SysMouseEm
			|| rguid == GUID_SysMouseEm2)
			return E_FAIL;
	}

	if (g_settings.DisableKeyboard)
	{
		if (rguid == GUID_SysKeyboard
			|| rguid == GUID_SysKeyboardEm
			|| rguid == GUID_SysKeyboardEm2)
			return E_FAIL;
	}

	HRESULT hr = m_original->CreateDevice(rguid, lplpDirectInputDevice, pUnkOuter);
	if (SUCCEEDED(hr))
	{
		auto device = new My_IDirectInputDevice8W(rguid, *lplpDirectInputDevice, this);
		*lplpDirectInputDevice = device;

		std::lock_guard<std::recursive_mutex> lock(m_lock);
		m_devices.insert(device);
	}

	return hr;
}

HRESULT My_IDirectInput8W::EnumDevices(DWORD dwDevType, LPDIENUMDEVICESCALLBACKW lpCallback, LPVOID pvRef, DWORD dwFlags)
{
	return m_original->EnumDevices(dwDevType, lpCallback, pvRef, dwFlags);
}

HRESULT My_IDirectInput8W::GetDeviceStatus(REFGUID rguidInstance)
{
	return m_original->GetDeviceStatus(rguidInstance);
}

HRESULT My_IDirectInput8W::RunControlPanel(HWND hwndOwner, DWORD dwFlags)
{
	return m_original->RunControlPanel(hwndOwner, dwFlags);
}

HRESULT My_IDirectInput8W::Initialize(HINSTANCE hinst, DWORD dwVersion)
{
	return m_original->Initialize(hinst, dwVersion);
}

HRESULT My_IDirectInput8W::FindDevice(REFGUID rguidClass, LPCWSTR ptszName, LPGUID pguidInstance)
{
	return m_original->FindDevice(rguidClass, ptszName, pguidInstance);
}

HRESULT My_IDirectInput8W::EnumDevicesBySemantics(LPCWSTR ptszUserName, LPDIACTIONFORMATW lpdiActionFormat, LPDIENUMDEVICESBYSEMANTICSCBW lpCallback, LPVOID pvRef, DWORD dwFlags)
{
	return m_original->EnumDevicesBySemantics(ptszUserName, lpdiActionFormat, lpCallback, pvRef, dwFlags);
}

HRESULT My_IDirectInput8W::ConfigureDevices(LPDICONFIGUREDEVICESCALLBACK lpdiCallback, LPDICONFIGUREDEVICESPARAMSW lpdiCDParams, DWORD dwFlags, LPVOID pvRefData)
{
	return m_original->ConfigureDevices(lpdiCallback, lpdiCDParams, dwFlags, pvRefData);
}

void My_IDirectInput8W::DetachDevice(My_IDirectInputDevice8W* device)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);
	m_devices.erase(device);
}
