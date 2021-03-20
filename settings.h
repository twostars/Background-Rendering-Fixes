#pragma once

struct Settings
{
	// Tricks the game into thinking it hasn't lost focus
	// so as to allow it to continue rendering in the background.
	// Background input will continue to be disregarded.
	bool UseBackgroundRendering = true;

	// For games that use DirectSound, they tend to by default have
	// no option to continue to play audio in the background.
	// This enables the 'global' flag to allow the game to provide background audio.
	bool UseBackgroundAudio = true;

	// For games that use XAudio2, they tend to not respect the per-app audio device setting
	// provided by Windows, instead using the system default.
	// This will hook XAudio2 to replace the endpoint with the per-app audio device if it thinks
	// it's requesting the system default (device index 0).
	bool UseAppAudioDevice = true;

	// Games sometimes try to clip your cursor inside the window.
	// When using borderless mode, this prevents you from actually clicking outside the window.
	// This disables such functionality, to allow your cursor to free roam.
	bool UseUnclippedCursor = true;

	// Tales of Vesperia worker/busy threads constantly poll every 1ms instead of simply waiting for a signal.
	// This unfortunately causes a noticeable microstutter in-game.
	// This fix is credited to Kaldaien (Special K mod), though his implementation is far more thorough than mine.
	bool TalesOfVesperia_MicroStutterFix = false;

	// DirectInput will ordinarily be hooked to handle background input behaviour.
	// Some games may not play nicely with this or otherwise detect it as malicious
	// so we should have the option to selectively turn it off.
	bool HookDirectInput = true;

	// DirectSound will ordinarily be hooked to handle background audio behaviour.
	// Some games may not play nicely with this or otherwise detect it as malicious
	// so we should have the option to selectively turn it off.
	bool HookDirectSound = true;

	bool Load(const wchar_t* processName, const wchar_t* configPath);
	void Save(const wchar_t* processName, const wchar_t* configPath) const;

	static const std::wstring& GetDefaultConfigPath();
	static void CreateDefault(const wchar_t* filename);
};

extern Settings g_settings;
