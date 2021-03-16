#include "stdafx.h"
#include "settings.h"
#include "utils.h"

Settings g_settings;

static const std::map<std::wstring, Settings, ci_less> DefaultSettings =
{
	// The Legend of Heroes: Trails in the Sky FC
	{ L"ed6_win",			{} },
	// The Legend of Heroes: Trails in the Sky FC
	{ L"ed6_win_DX9",		{} },
	// The Legend of Heroes: Trails in the Sky SC
	{ L"ed6_win2",			{} },
	// The Legend of Heroes: Trails in the Sky SC
	{ L"ed6_win2_DX9",		{} },
	// The Legend of Heroes: Trails in the Sky the 3rd
	{ L"ed6_win3",			{} },
	// The Legend of Heroes: Trails in the Sky the 3rd
	{ L"ed6_win3_DX9",		{} },
	// The Legend of Heroes: Trails of Cold Steel I
	{ L"ed8",				{} },
	// The Legend of Heroes: Trails of Cold Steel I
	{ L"ed8jp",				{} },
	// The Legend of Heroes: Trails of Cold Steel II
	{ L"ed8_2_PC_US",		{} },
	// The Legend of Heroes: Trails of Cold Steel II
	{ L"ed8_2_PC_JP",		{} },
	// The Legend of Heroes: Trails of Cold Steel III
	{ L"ed8_3_PC",			{} },
	// The Legend of Heroes: Trails of Cold Steel III
	{ L"ed8_3_PC_JP",		{} },
	// Final Fantasy XIII
	{ L"ffxiiiimg",			{} },
	// A Hat in Time
	{ L"HatinTimeGame",		{} },
	// Valheim
	{ L"valheim",			{} },
	// Tales of Vesperia: Definitive Edition
	{ L"TOV_DE",			{} }
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

	UINT enabled = GetPrivateProfileInt(processName, L"Enabled", UINT_MAX, configPath);
	if (enabled == 0)
		return false;

	if (enabled == UINT_MAX)
	{
		// Check to see if this process exists as a default.
		// If so, it was added after the file was created and should be respected.
		auto itr = DefaultSettings.find(processName);
		if (itr == DefaultSettings.end())
			return false;

		itr->second.Save(itr->first.c_str(), configPath);
	}

	UseBackgroundRendering = GetPrivateProfileInt(processName, L"UseBackgroundRendering", 1, configPath) != 0;
	UseBackgroundAudio = GetPrivateProfileInt(processName, L"UseBackgroundAudio", 1, configPath) != 0;
	UseAppAudioDevice = GetPrivateProfileInt(processName, L"UseAppAudioDevice", 1, configPath) != 0;
	UseUnclippedCursor = GetPrivateProfileInt(processName, L"UseUnclippedCursor", 1, configPath) != 0;

	return true;
}

void Settings::Save(const wchar_t* processName, const wchar_t* configPath) const
{
	WritePrivateProfileString(processName, L"Enabled", L"1", configPath);
	WritePrivateProfileString(processName, L"UseBackgroundRendering", UseBackgroundRendering ? L"1" : L"0", configPath);
	WritePrivateProfileString(processName, L"UseBackgroundAudio", UseBackgroundAudio ? L"1" : L"0", configPath);
	WritePrivateProfileString(processName, L"UseAppAudioDevice", UseAppAudioDevice ? L"1" : L"0", configPath);
	WritePrivateProfileString(processName, L"UseUnclippedCursor", UseUnclippedCursor ? L"1" : L"0", configPath);
}

const std::wstring& Settings::GetDefaultConfigPath()
{
	static const std::wstring DefaultConfigPath = GetStoragePath() + L"\\Background-Rendering-Fixes.ini";
	return DefaultConfigPath;
}

void Settings::CreateDefault(const wchar_t* configPath)
{
	for (auto itr = DefaultSettings.begin(); itr != DefaultSettings.end(); ++itr)
		itr->second.Save(itr->first.c_str(), configPath);
}
