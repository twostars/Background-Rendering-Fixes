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
	// The Legend of Heroes: Trails to Azure
	{ L"ED_AO",				{} },
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
	// The Legend of Heroes: Trails of Cold Steel IV
	{ L"ed8_4_PC",			{ .UseBackgroundAudio = true } },
	// The Legend of Heroes: Trails of Cold Steel IV
	{ L"ed8_4_PC_JP",		{ .UseBackgroundAudio = true } },
	// Final Fantasy XIII
	{ L"ffxiiiimg",			{} },
	// A Hat in Time
	{ L"HatinTimeGame",		{} },
	// Valheim
	{ L"valheim",			{} },
	// Tales of Vesperia: Definitive Edition
	{ L"TOV_DE",			{ .TalesOfVesperia_MicroStutterFix = true } },
	// Okami HD
	{ L"okami",				{} },
	// Tell Me Why
	{ L"TME-Win64-Shipping", {} },
	// Persona 5: Strikers
	{ L"game.exe", {} }
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

	UINT enabled = GetPrivateProfileIntW(processName, L"Enabled", UINT_MAX, configPath);
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

	LogLevel = GetPrivateProfileIntW(processName, L"LogLevel", LOGLEVEL_INFO, configPath) != 0;
	WindowHooks = GetPrivateProfileIntW(processName, L"WindowHooks", 1, configPath) != 0;

	UseBackgroundRendering = GetPrivateProfileIntW(processName, L"UseBackgroundRendering", 1, configPath) != 0;
	UseBackgroundAudio = GetPrivateProfileIntW(processName, L"UseBackgroundAudio", 0, configPath) != 0;
	UseAppAudioDevice = GetPrivateProfileIntW(processName, L"UseAppAudioDevice", 1, configPath) != 0;
	UseUnclippedCursor = GetPrivateProfileIntW(processName, L"UseUnclippedCursor", 1, configPath) != 0;

	TalesOfVesperia_MicroStutterFix = GetPrivateProfileIntW(processName, L"TalesOfVesperia.MicroStutterFix", 0, configPath) != 0;

	HookDirectInput = GetPrivateProfileIntW(processName, L"HookDirectInput", 1, configPath) != 0;
	HookDirectSound = GetPrivateProfileIntW(processName, L"HookDirectSound", 1, configPath) != 0;

	DisableMouse = GetPrivateProfileIntW(processName, L"DisableMouse", 0, configPath) != 0;
	DisableKeyboard = GetPrivateProfileIntW(processName, L"DisableKeyboard", 0, configPath) != 0;

	return true;
}

void Settings::Save(const wchar_t* processName, const wchar_t* configPath) const
{
	static const Settings DefaultSettings;

	WritePrivateProfileStringW(processName, L"Enabled", L"1", configPath);

	if (LogLevel != DefaultSettings.LogLevel)
	{
		auto logLevel = std::to_wstring(LogLevel);
		WritePrivateProfileStringW(processName, L"LogLevel", logLevel.c_str(), configPath);
	}

	if (UseBackgroundRendering != DefaultSettings.UseBackgroundRendering)
		WritePrivateProfileStringW(processName, L"UseBackgroundRendering", UseBackgroundRendering ? L"1" : L"0", configPath);

	if (UseBackgroundAudio != DefaultSettings.UseBackgroundAudio)
		WritePrivateProfileStringW(processName, L"UseBackgroundAudio", UseBackgroundAudio ? L"1" : L"0", configPath);

	if (UseAppAudioDevice != DefaultSettings.UseAppAudioDevice)
		WritePrivateProfileStringW(processName, L"UseAppAudioDevice", UseAppAudioDevice ? L"1" : L"0", configPath);

	if (UseUnclippedCursor != DefaultSettings.UseUnclippedCursor)
		WritePrivateProfileStringW(processName, L"UseUnclippedCursor", UseUnclippedCursor ? L"1" : L"0", configPath);

	if (TalesOfVesperia_MicroStutterFix != DefaultSettings.TalesOfVesperia_MicroStutterFix)
		WritePrivateProfileStringW(processName, L"TalesOfVesperia.MicroStutterFix", TalesOfVesperia_MicroStutterFix ? L"1" : L"0", configPath);

	if (HookDirectInput != DefaultSettings.HookDirectInput)
		WritePrivateProfileStringW(processName, L"HookDirectInput", HookDirectInput ? L"1" : L"0", configPath);

	if (HookDirectSound != DefaultSettings.HookDirectSound)
		WritePrivateProfileStringW(processName, L"HookDirectSound", HookDirectSound ? L"1" : L"0", configPath);

	if (DisableMouse != DefaultSettings.DisableMouse)
		WritePrivateProfileStringW(processName, L"DisableMouse", DisableMouse ? L"1" : L"0", configPath);

	if (DisableKeyboard != DefaultSettings.DisableKeyboard)
		WritePrivateProfileStringW(processName, L"DisableKeyboard", DisableKeyboard ? L"1" : L"0", configPath);
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
