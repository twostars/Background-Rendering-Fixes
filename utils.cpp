#include "stdafx.h"
#include <ShlObj.h>

bool ci_less::operator()(const std::wstring& str1, const std::wstring& str2) const
{
	return _wcsicmp(str1.c_str(), str2.c_str()) < 0;
}

const std::wstring& GetExePath()
{
	static bool loaded = false;
	static std::wstring path;

	if (!loaded)
	{
		std::vector<wchar_t> pathBuffer;
		DWORD copied = 0;
		do
		{
			pathBuffer.resize(pathBuffer.size() + MAX_PATH);
			copied = GetModuleFileNameW(0, pathBuffer.data(), pathBuffer.size());
		} while (copied >= pathBuffer.size());
		pathBuffer.resize(copied);

		path.assign(pathBuffer.begin(), pathBuffer.end());
	}

	return path;
}

const std::wstring& GetProcessName()
{
	static bool loaded = false;
	static std::wstring processName;

	if (!loaded)
	{
		wchar_t filename[_MAX_FNAME] = { 0 };
		_wsplitpath_s(GetExePath().c_str(), nullptr, 0, nullptr, 0, filename, _MAX_FNAME, nullptr, 0);
		processName = filename;
	}

	return processName;
}

const std::wstring& GetRoamingAppDataPath()
{
	static bool loaded = false;
	static std::wstring appDataPath;

	if (!loaded)
	{
		PWSTR path;

		auto ret = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &path);
		if (ret == S_OK)
			appDataPath = path;

		CoTaskMemFree(path);
	}

	return appDataPath;
}

const std::wstring& GetStoragePath()
{
	static const std::wstring path = GetRoamingAppDataPath() + L"\\Background-Rendering-Fixes";
	return path;
}

bool ExtractDeviceIDFromMMDevPath(const std::wstring& path, std::wstring* deviceID)
{
	// Sample string: \\?\SWD#MMDEVAPI#{0.0.0.00000000}.{fd9d7031-4c5d-4d34-be9b-0ec5a5168f8f}#{e6327cad-dcec-4949-ae8a-991e976a79d2}
	// Device ID (as reported by XAudio2): {0.0.0.00000000}.{fd9d7031-4c5d-4d34-be9b-0ec5a5168f8f}
	size_t startPos = path.find(L"#{");
	if (startPos == std::wstring::npos)
		return false;

	++startPos; /* skip to start of next block, including the first { */
	size_t endPos = path.find(L"}#", startPos);
	if (endPos == std::wstring::npos)
		return false;

	++endPos; /* skip to after the block, so as to include the } */
	*deviceID = path.substr(startPos, endPos - startPos);

	return true;
}

bool GetDefaultAudioEndpointPathForProcess(const std::wstring& processPath, std::wstring* mmdevPath)
{
	if (processPath.size() < 2)
		return false;

	// As of Windows 10 (1803), per-application defaults are now a thing.
	// These are stored in the registry as such:
	// HKCU\SOFTWARE\Microsoft\Multimedia\Audio\DefaultEndpoint\(unknown key name)
	// Example key: b35f99e1_0
	// (default): DOS  path for the applicable process, e.g.:
	// \Device\HarddiskVolume4\SteamLibrary\SteamApps\common\The Legend of Heroes Trails of Cold Steel III\bin\x64\ed8_3_PC.exe
	// + 000_000: the MM device path for the output endpoint, e.g.:
	// \\?\SWD#MMDEVAPI#{0.0.0.00000000}.{fd9d7031-4c5d-4d34-be9b-0ec5a5168f8f}#{e6327cad-dcec-4949-ae8a-991e976a79d2}
	// + 000_000_p: unknown, e.g.:
	// {1CF8EA30-07C5-4556-8129-5E99DDF893FB}
	// + 000_001: the MM device path for the output endpoint
	// + 000_001_p: unknown
	// Also: 001_* are for input
	// If it isn't changed from the default, its applicable 4 keys won't exist.
	// I don't know what the difference is between *_000 and *_001, they both get set to the same data.
	// As I also don't know how the key works, we will be forced to enumerate all keys and check each path.

	// Extract the drive from the process path (e.g. "D:")
	WCHAR driveName[_MAX_DRIVE] = { 0 };
	_wsplitpath(processPath.c_str(), driveName, nullptr, nullptr, nullptr);

	// Convert the drive (e.g. "D:") to its DOS path equivalent (e.g. "\Device\HarddiskVolume4")
	WCHAR dosDeviceName[MAX_PATH] = { 0 };
	if (QueryDosDeviceW(driveName, dosDeviceName, MAX_PATH) == 0)
		return false;

	// "Convert" our translated path (e.g. "D:\SteamLibrary\...") to its DOS path equivalent (e.g. "\Device\HarddiskVolume4\SteamLibrary\...")
	// as Windows stores it in the registry, so we can use it for finding an applicable match.
	std::wstring actualDosPath = dosDeviceName + processPath.substr(2);

	const std::wstring BaseSubkey = L"SOFTWARE\\Microsoft\\Multimedia\\Audio\\DefaultEndpoint";

	HKEY hKey;
	if (RegOpenKeyExW(
		HKEY_CURRENT_USER,
		BaseSubkey.c_str(),
		0,
		KEY_READ | KEY_WOW64_64KEY,
		&hKey) != ERROR_SUCCESS)
		return false;

	const int MAX_KEY_LENGTH = 255;
	const int MAX_DOS_PATH = 16383;

	WCHAR achClass[MAX_PATH] = L"";	// buffer for class name 
	DWORD cchClassName = MAX_PATH;	// size of class string 
	DWORD cSubKeys = 0;				// number of subkeys 

	LSTATUS retCode = RegQueryInfoKeyW(
		hKey,					// key handle 
		achClass,				// buffer for class name 
		&cchClassName,			// size of class string 
		nullptr,				// reserved 
		&cSubKeys,				// number of subkeys 
		nullptr,				// longest subkey size 
		nullptr,				// longest class string 
		nullptr,				// number of values for this key 
		nullptr,				// longest value name 
		nullptr,				// longest value data 
		nullptr,				// security descriptor 
		nullptr);				// last write time 

	if (retCode != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return false;
	}

	bool foundMatchingEndpoint = false;
	for (DWORD i = 0; i < cSubKeys; i++)
	{
		DWORD cbName = MAX_KEY_LENGTH;
		WCHAR achKey[MAX_KEY_LENGTH] = { 0 };

		retCode = RegEnumKeyExW(
			hKey,
			i,
			achKey,
			&cbName,
			nullptr,
			nullptr,
			nullptr,
			nullptr);

		if (retCode != ERROR_SUCCESS)
			continue;

		HKEY hSubkey;

		std::wstring subKey = BaseSubkey + L"\\";
		subKey += achKey;

		if (RegOpenKeyExW(
			HKEY_CURRENT_USER,
			subKey.c_str(),
			0,
			KEY_READ | KEY_WOW64_64KEY,
			&hSubkey) != ERROR_SUCCESS)
			continue;

		DWORD dwType = REG_SZ;
		DWORD dwSize = MAX_DOS_PATH;
		WCHAR dosPath[MAX_DOS_PATH] = { 0 };

		retCode = RegQueryValueExW(
			hSubkey,
			nullptr,
			nullptr,
			&dwType,
			(LPBYTE)dosPath,
			&dwSize);

		if (retCode != ERROR_SUCCESS
			|| wcsicmp(dosPath, actualDosPath.c_str()) != 0)
		{
			RegCloseKey(hSubkey);
			continue;
		}

		WCHAR mmdevPath_[MAX_DOS_PATH] = { 0 };
		dwType = REG_SZ;
		dwSize = MAX_DOS_PATH;
		retCode = RegQueryValueExW(
			hSubkey,
			L"000_000",
			nullptr,
			&dwType,
			(LPBYTE)mmdevPath_,
			&dwSize);

		RegCloseKey(hSubkey);

		if (retCode != ERROR_SUCCESS)
			continue;

		*mmdevPath = mmdevPath_;
		foundMatchingEndpoint = true;
		break;
	}

	RegCloseKey(hKey);
	return foundMatchingEndpoint;
}

bool GetDefaultAudioEndpointPathForThisProcess(std::wstring* mmdevPath)
{
	const std::wstring& processPath = GetExePath();
	return GetDefaultAudioEndpointPathForProcess(processPath, mmdevPath);
}

bool GetDefaultAudioEndpointIDForProcess(const std::wstring& processPath, std::wstring* deviceID)
{
	std::wstring mmdevPath;
	if (!GetDefaultAudioEndpointPathForProcess(processPath, &mmdevPath))
		return false;

	return ExtractDeviceIDFromMMDevPath(mmdevPath, deviceID);
}

bool GetDefaultAudioEndpointIDForThisProcess(std::wstring* deviceID)
{
	const std::wstring& processPath = GetExePath();
	return GetDefaultAudioEndpointIDForProcess(processPath, deviceID);
}

void WriteLog(const wchar_t* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	WriteLogEx(fmt, args);
	va_end(args);
}

void WriteLogEx(const wchar_t* fmt, va_list args)
{
	auto path = GetStoragePath() + L"\\log.txt";
	FILE* fp = _wfopen(path.c_str(), L"a");
	if (fp != nullptr)
	{
		vfwprintf(fp, fmt, args);
		fclose(fp);
	}

	wchar_t buffer[1024] = { 0 };
	vswprintf(buffer, fmt, args);
	OutputDebugStringW(buffer);
}
