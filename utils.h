#pragma once

struct ci_less
{
	bool operator()(const std::wstring& str1, const std::wstring& str2) const;
};

const std::wstring& GetExePath();
const std::wstring& GetProcessName();

bool ExtractDeviceIDFromMMDevPath(const std::wstring& path, std::wstring* deviceID);
bool GetDefaultAudioEndpointPathForProcess(const std::wstring& processPath, std::wstring* mmdevPath);
bool GetDefaultAudioEndpointPathForThisProcess(std::wstring* mmdevPath);
bool GetDefaultAudioEndpointIDForProcess(const std::wstring& processPath, std::wstring* deviceID);
bool GetDefaultAudioEndpointIDForThisProcess(std::wstring* deviceID);
