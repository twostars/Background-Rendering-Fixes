#pragma once

struct ci_less
{
	bool operator()(const std::wstring& str1, const std::wstring& str2) const;
};

const std::wstring& GetExePath();
const std::wstring& GetProcessName();
const std::wstring& GetRoamingAppDataPath();
const std::wstring& GetStoragePath();

bool ExtractDeviceIDFromMMDevPath(const std::wstring& path, std::wstring* deviceID);
bool GetDefaultAudioEndpointPathForProcess(const std::wstring& processPath, std::wstring* mmdevPath);
bool GetDefaultAudioEndpointPathForThisProcess(std::wstring* mmdevPath);
bool GetDefaultAudioEndpointIDForProcess(const std::wstring& processPath, std::wstring* deviceID);
bool GetDefaultAudioEndpointIDForThisProcess(std::wstring* deviceID);

void WriteLogA(const char* fmt, ...);
void WriteLogExA(const char* fmt, va_list args);

void WriteLogW(const wchar_t* fmt, ...);
void WriteLogExW(const wchar_t* fmt, va_list args);

#define WriteLog WriteLogW
#define WriteLogEx WriteLogExW
