#pragma once

#include <FMOD/fmod.hpp>
#include <FMOD/fmod_errors.h>
#include <string>
#include <map>

class AudioManager
{
public:
	FMOD_RESULT fResult;
	FMOD::System* audio_system;

	std::map<std::string, FMOD::ChannelGroup*> channelGroups;
	std::map<std::string, FMOD::Sound*> sounds;

	AudioManager();
	~AudioManager();

	bool Initialize(const int numberOfChannels, const int systemFlags);
	void Shutdown();

	bool CreateChannelGroup(const std::string& name);
	bool SetChannelGroupVolume(const std::string& name, float volume);
	bool GetChannelGroupVolume(const std::string& name, float* volume);
	bool CreateSound(const std::string& soundName, const std::string& path, const int mode);
	bool PlaySound(const std::string& soundName, const std::string& channelName);
};

