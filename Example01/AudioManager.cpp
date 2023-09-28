#include "AudioManager.h"

#include <iostream>


gdpAudioNamespace_Begin

AudioManager::AudioManager()
{
	// Usually do nothing that requires code execution
	// Declare initial values for everything
	// Utilize Initialize function instead
}

AudioManager::~AudioManager()
{
	// Typically just for notifying things that this is being destroyed.
	// Utilize the Destroy function instead
}

void AudioManager::Initialize()
{
	if (m_Initialized)
		return;

	FMOD_RESULT result;
	result = FMOD::System_Create(&m_System);
	if (result != FMOD_OK)
	{
		printf("Failed to create the FMOD System!\n");
		return;
	}

	result = m_System->init(3, FMOD_INIT_NORMAL, nullptr);
	if (result != FMOD_OK)
	{
		printf("Failed to initialize the system!\n");
		// Cleanup
		result = m_System->close();
		if (result != FMOD_OK)
		{
			printf("Failed to close system!\n");
		}
		return;
	}

	printf("AudioManager::Initialize(): Successful!\n");

	for (int i = 0; i < 10; i++)
	{
		m_ChannelList.push_back(new Channel);
	}

	m_Initialized = true;
}

void AudioManager::Destroy()
{
	if (!m_Initialized)
		return;

	FMOD_RESULT result;

	// Move this to a function, clear and release Audios from Audio map
	for (std::pair<const char*, Audio*> pair : m_AudioMap)
	{
		result = pair.second->Audio->release();
		FMODCheckError(result);
	}

	m_AudioMap.clear();

	result = m_System->close();
	FMODCheckError(result);

	result = m_System->release();
	FMODCheckError(result);

	m_Initialized = false;
}

// We can also load an audio file as a streaming asset
// This will only load the audio resource data as needed
// to play the audio file.
void AudioManager::LoadAudioStream(const char* source)
{
	if (!m_Initialized)
		return;

	if (m_AudioMap.find(source) != m_AudioMap.end())
	{
		printf("AudioManager::LoadAudio() Audio already loaded!\n");
		return;
	}

	m_AudioMap.insert(std::pair<const char*, Audio*>(source, new Audio()));

	FMOD_RESULT result;
	result = m_System->createStream(source, FMOD_DEFAULT, 0, &m_AudioMap[source]->Audio);
	if (result != FMOD_OK)
	{
		printf("AudioManager::LoadAudio() Failed to load the Audio stream: %s\n", source);
	}

	printf("AudioManager::LoadAudio(%s): Loaded successful!\n", source);
}

// This function loads a media file directly into memory
void AudioManager::LoadAudio(const char* file)
{
	if (!m_Initialized)
		return;

	if (m_AudioMap.find(file) != m_AudioMap.end())
	{
		printf("AudioManager::LoadAudio() Audio already loaded!\n");
		return;
	}

	m_AudioMap.insert(std::pair<const char*, Audio*>(file, new Audio()));

	FMOD_RESULT result;

	// This call loads our audio file entirely into memory
	result = m_System->createSound(file, FMOD_DEFAULT, 0, &m_AudioMap[file]->Audio);
	if (result != FMOD_OK)
	{
		printf("AudioManager::LoadAudio() Failed to load the Audio file: %s\n", file);
	}

	printf("AudioManager::LoadAudio(%s): Loaded successful!\n", file);
}

int AudioManager::PlayAudio(const char* AudioName)
{
	if (!m_Initialized)
	{
		printf("Not initialized!\n");
		return m_NextChannelId;	// Still want to return a valid channel id
	}
	std::map<const char*, Audio*>::iterator it = m_AudioMap.find(AudioName);
	if (it == m_AudioMap.end())
	{
		printf("Audio not found!\n");
		return m_NextChannelId;	// Still want to return a valid channel id
	}
	printf("AudioFound!\n");

	// Get the current channel id, and calculate the next one
	int channelId = m_NextChannelId;
	m_NextChannelId = (m_NextChannelId + 1) % 10;
	Channel* channel = m_ChannelList[channelId];

	FMOD_RESULT result;
	result = m_System->playSound(it->second->Audio, 0, false, &channel->fmodChannel);
	FMODCheckError(result);

	// Return the channel id used so we can modify it if needed.
	return channelId;
}

void AudioManager::Update()
{
	if (!m_Initialized)
		return;

	FMOD_RESULT result;
	result = m_System->update();
	if (result != FMOD_OK)
	{
		FMODCheckError(result);
		Destroy();
	}
}

void AudioManager::SetChannelVolume(int id, float value)
{
	FMOD_RESULT result = m_ChannelList[id]->fmodChannel->setVolume(value);
	FMODCheckError(result);
}

void AudioManager::SetChannelPitch(int id, float value)
{
	FMOD_RESULT result = m_ChannelList[id]->fmodChannel->setPitch(value);
	FMODCheckError(result);
}

void AudioManager::SetChannelPan(int id, float value)
{
	FMOD_RESULT result = m_ChannelList[id]->fmodChannel->setPan(value);
	FMODCheckError(result);
}

bool AudioManager::GetChannelPlaying(int id)
{
	bool isPlaying;
	FMOD_RESULT result = m_ChannelList[id]->fmodChannel->isPlaying(&isPlaying);
	return isPlaying;
}

void AudioManager::GetPlaybackPosition(int id, unsigned int& value)
{
	FMOD_RESULT result = m_ChannelList[id]->fmodChannel->getPosition(&value, FMOD_TIMEUNIT_MS);
	FMODCheckError(result);
}

void AudioManager::PrintInfo() const
{
	size_t size = m_ChannelList.size();

	for (size_t i = 0; i < m_ChannelList.size(); i++)
	{
	}
}

gdpAudioNamespace_End