#pragma once

struct Settings
{
	// Tricks the game into thinking it hasn't lost focus
	// so as to allow it to continue rendering in the background.
	// Background input will continue to be disregarded.
	bool UseBackgroundRendering;

	// For games that use DirectSound, they tend to by default have
	// no option to continue to play audio in the background.
	// This enables the 'global' flag to allow the game to provide background audio.
	bool UseBackgroundAudio;

	// For games that use XAudio2, they tend to not respect the per-app audio device setting
	// provided by Windows, instead using the system default.
	// This will hook XAudio2 to replace the endpoint with the per-app audio device if it thinks
	// it's requesting the system default (device index 0).
	bool UseAppAudioDevice;

	bool Load(const wchar_t* processName, const wchar_t* configPath);
	void Save(const wchar_t* processName, const wchar_t* configPath) const;

	static const std::wstring& GetDefaultConfigPath();
	static void CreateDefault(const wchar_t* filename);
};

extern Settings g_settings;
