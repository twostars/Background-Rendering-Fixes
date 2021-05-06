#include "stdafx.h"
#include "settings.h"

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
	// The Legend of Heroes: Trails from Zero
	{ L"ED_ZERO",			{} },
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
	{ L"TOV_DE",			{ .TalesOfVesperia_MicroStutterFix = true } },
	// Okami HD
	{ L"okami",				{} }
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

	TalesOfVesperia_MicroStutterFix = GetPrivateProfileInt(processName, L"TalesOfVesperia.MicroStutterFix", 0, configPath) != 0;

	HookDirectInput = GetPrivateProfileInt(processName, L"HookDirectInput", 1, configPath) != 0;
	HookDirectSound = GetPrivateProfileInt(processName, L"HookDirectSound", 1, configPath) != 0;

	return true;
}

void Settings::Save(const wchar_t* processName, const wchar_t* configPath) const
{
	static const Settings DefaultSettings;

	WritePrivateProfileString(processName, L"Enabled", L"1", configPath);

	if (UseBackgroundRendering != DefaultSettings.UseBackgroundRendering)
		WritePrivateProfileString(processName, L"UseBackgroundRendering", UseBackgroundRendering ? L"1" : L"0", configPath);

	if (UseBackgroundAudio != DefaultSettings.UseBackgroundAudio)
		WritePrivateProfileString(processName, L"UseBackgroundAudio", UseBackgroundAudio ? L"1" : L"0", configPath);

	if (UseAppAudioDevice != DefaultSettings.UseAppAudioDevice)
		WritePrivateProfileString(processName, L"UseAppAudioDevice", UseAppAudioDevice ? L"1" : L"0", configPath);

	if (UseUnclippedCursor != DefaultSettings.UseUnclippedCursor)
		WritePrivateProfileString(processName, L"UseUnclippedCursor", UseUnclippedCursor ? L"1" : L"0", configPath);

	if (TalesOfVesperia_MicroStutterFix != DefaultSettings.TalesOfVesperia_MicroStutterFix)
		WritePrivateProfileString(processName, L"TalesOfVesperia.MicroStutterFix", TalesOfVesperia_MicroStutterFix ? L"1" : L"0", configPath);

	if (HookDirectInput != DefaultSettings.HookDirectInput)
		WritePrivateProfileString(processName, L"HookDirectInput", HookDirectInput ? L"1" : L"0", configPath);

	if (HookDirectSound != DefaultSettings.HookDirectSound)
		WritePrivateProfileString(processName, L"HookDirectSound", HookDirectSound ? L"1" : L"0", configPath);
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
