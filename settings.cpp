#include "stdafx.h"
#include "settings.h"
#include "utils.h"

Settings g_settings;

static const std::map<std::wstring, Settings, ci_less> DefaultSettings =
{
	//                     BGInput BGAudio AppAudio
	// The Legend of Heroes: Trails in the Sky FC
	{ L"ed6_win",			{ true, true, true } },
	// The Legend of Heroes: Trails in the Sky FC
	{ L"ed6_win_DX9",		{ true, true, true } },
	// The Legend of Heroes: Trails in the Sky SC
	{ L"ed6_win2",			{ true, true, true } },
	// The Legend of Heroes: Trails in the Sky SC
	{ L"ed6_win2_DX9",		{ true, true, true } },
	// The Legend of Heroes: Trails in the Sky the 3rd
	{ L"ed6_win3",			{ true, true, true } },
	// The Legend of Heroes: Trails in the Sky the 3rd
	{ L"ed6_win3_DX9",		{ true, true, true } },
	// The Legend of Heroes: Trails of Cold Steel I
	{ L"ed8",				{ true, true, true } },
	// The Legend of Heroes: Trails of Cold Steel I
	{ L"ed8jp",				{ true, true, true } },
	// The Legend of Heroes: Trails of Cold Steel II
	{ L"ed8_2_PC_US",		{ true, true, true } },
	// The Legend of Heroes: Trails of Cold Steel II
	{ L"ed8_2_PC_JP",		{ true, true, true } },
	// The Legend of Heroes: Trails of Cold Steel III
	{ L"ed8_3_PC",			{ true, true, true } },
	// The Legend of Heroes: Trails of Cold Steel III
	{ L"ed8_3_PC_JP",		{ true, true, true } },
	// Final Fantasy XIII
	{ L"ffxiiiimg",			{ true, true, true } },
	// A Hat in Time
	{ L"HatinTimeGame",		{ false, false, true } },
	// Valheim
	{ L"valheim",			{ false, false, true } },
};

bool Settings::Load(const wchar_t* processName, const wchar_t* configPath)
{
	std::error_code ec;
	if (!std::filesystem::exists(configPath, ec))
	{
		std::filesystem::path path(configPath), parentPath = path.parent_path();
		if (!std::filesystem::exists(parentPath, ec))
			std::filesystem::create_directories(parentPath, ec);

		CreateDefault(configPath);
	}

	if (GetPrivateProfileInt(processName, L"Enabled", 0, configPath) == 0)
	{
		// Check to see if this process exists as a default.
		// If so, it was added after the file was created and should be respected.
		auto itr = DefaultSettings.find(processName);
		if (itr == DefaultSettings.end())
			return false;

		itr->second.Save(itr->first.c_str(), configPath);
	}

	UseBackgroundRendering = GetPrivateProfileInt(processName, L"UseBackgroundRendering", 0, configPath) != 0;
	UseBackgroundAudio = GetPrivateProfileInt(processName, L"UseBackgroundAudio", 0, configPath) != 0;
	UseAppAudioDevice = GetPrivateProfileInt(processName, L"UseAppAudioDevice", 0, configPath) != 0;

	return true;
}

void Settings::Save(const wchar_t* processName, const wchar_t* configPath) const
{
	WritePrivateProfileString(processName, L"Enabled", L"1", configPath);
	WritePrivateProfileString(processName, L"UseBackgroundRendering", UseBackgroundRendering ? L"1" : L"0", configPath);
	WritePrivateProfileString(processName, L"UseBackgroundAudio", UseBackgroundAudio ? L"1" : L"0", configPath);
	WritePrivateProfileString(processName, L"UseAppAudioDevice", UseAppAudioDevice ? L"1" : L"0", configPath);
}

const std::wstring& Settings::GetDefaultConfigPath()
{
	static const std::wstring DefaultConfigPath = GetRoamingAppDataPath() + L"\\Background-Rendering-Fixes\\Background-Rendering-Fixes.ini";
	return DefaultConfigPath;
}

void Settings::CreateDefault(const wchar_t* configPath)
{
	for (auto itr = DefaultSettings.begin(); itr != DefaultSettings.end(); ++itr)
		itr->second.Save(itr->first.c_str(), configPath);
}
