#include "AudioManager.h"

AudioManager::AudioManager()
{
	
}

AudioManager::~AudioManager()
{
	this->Shutdown();
}

bool AudioManager::Initialize(const int numberOfChannels, const int systemFlags)
{
	fResult = FMOD::System_Create(&audio_system);
	if (fResult != FMOD_OK)
	{
		return false;
	}
	fResult = audio_system->init(numberOfChannels, systemFlags, nullptr);
	if (fResult != FMOD_OK)
	{
		return false;
	}

	return true;
}

bool AudioManager::CreateChannelGroup(const std::string& name)
{
	FMOD::ChannelGroup* channel_group;
	fResult = audio_system->createChannelGroup(name.c_str(), &channel_group);
	if (fResult != FMOD_OK)
	{
		return false;
	}

	channelGroups.try_emplace(name, channel_group);

	return true;
}

bool AudioManager::GetChannelGroupVolume(const std::string& name, float* volume)
{
	const auto iterator = channelGroups.find(name);
	if (iterator == channelGroups.end())
		return false;

	fResult = iterator->second->getVolume(volume);
	return fResult == FMOD_OK;
}

bool AudioManager::SetChannelGroupVolume(const std::string& name, float volume)
{
	const auto iterator = channelGroups.find(name);
	if (iterator == channelGroups.end())
		return false;

	fResult = iterator->second->setVolume(volume);
	return fResult == FMOD_OK;
}

bool AudioManager::CreateSound(const std::string& name, const std::string& path, const int mode)
{
	FMOD::Sound* sound;
	fResult = audio_system->createSound(path.c_str(), mode, nullptr, &sound);
	if (fResult != FMOD_OK)
	{
		return false;
	}

	sounds.try_emplace(name, sound);

	return true;
}

bool AudioManager::PlaySound(const std::string& sound_name, const std::string& channel_group_name)
{
	const auto sound_iterator = sounds.find(sound_name);
	const auto channel_group_iterator = channelGroups.find(channel_group_name);

	if (sound_iterator == sounds.end() || channel_group_iterator == channelGroups.end())
	{
		return false;
	}

	FMOD::Channel* channel;
	fResult = audio_system->playSound(sound_iterator->second, 
		channel_group_iterator->second, true, &channel);
	if (fResult != FMOD_OK)
	{
		return false;
	}

	fResult = (*channel).setPaused(false);
	if (fResult != FMOD_OK)
	{
		return false;
	}

	return true;
}

void AudioManager::Shutdown()
{
	if (audio_system)
	{
		audio_system->release();
		audio_system = nullptr;
	}
}