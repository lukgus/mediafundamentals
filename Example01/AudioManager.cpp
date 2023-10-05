#include "AudioManager.h"

#include <iostream>


namespace audio
{
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

		result = m_System->init(30, FMOD_INIT_NORMAL, nullptr);
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

		for (int i = 0; i < 20; i++)
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

	/**
	  * LoadAudio - Checks if the provided file is already loaded in our
	  * cache, if it is not in our cache we can load it.
	  * 
	  */
	void AudioManager::LoadAudio(const char* file)
	{
		// If our AudioManager is not initialized, we shouldn't do anything
		if (!m_Initialized)
			return;

		// Search our cache if we have already loaded this file
		// If we have already loaded the file, return
		if (m_AudioMap.find(file) != m_AudioMap.end())
		{
			printf("AudioManager::LoadAudio() Audio already loaded!\n");
			return;
		}

		// Since we have not already loaded this file, we can add it to our
		// AudioMap as a cached value.
		// We can add this first so we can reference the audio file when
		// Creating the sound.
		m_AudioMap.insert(std::pair<const char*, Audio*>(file, new Audio()));

		// This call loads our audio file entirely into memory
		FMOD_RESULT result = m_System->createSound(file, FMOD_DEFAULT, 0, &m_AudioMap[file]->Audio);
		if (result != FMOD_OK)
		{
			// Failed to load the audio file.
			printf("AudioManager::LoadAudio() Failed to load the Audio file: %s\n", file);
			return;
		}

		printf("AudioManager::LoadAudio(%s): Loaded successful!\n", file);
	}

	int AudioManager::PlayAudio(const char* AudioName)
	{
		// If our AudioManager is not initialized, we shouldn't do anything
		if (!m_Initialized)
		{
			printf("Not initialized!\n");
			return m_NextChannelId;	// Still want to return a valid channel id
		}

		// Check our cache to see if the audio exists.
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

		// Attemp to play our sound.
		FMOD_RESULT result = m_System->playSound(it->second->Audio, 0, false, &channel->fmodChannel);
		FMODCheckError(result);

		// Return the channel id used so we can modify it if needed.
		return channelId;
	}

	void AudioManager::Update()
	{
		// If our AudioManager is not initialized, we shouldn't do anything
		if (!m_Initialized)
		{
			return;
		}

		// Call system update, this needs to occur once in a while. It is 
		// not mandatory for each frame.
		FMOD_RESULT result = m_System->update();

		if (result != FMOD_OK)
		{
			FMODCheckError(result);
			Destroy();
		}
	}

	void AudioManager::SetChannelVolume(int id, float value)
	{
		// This call sets the volume on the channel. We can't modify 
		// the value, we can only set it.
		FMOD_RESULT result = m_ChannelList[id]->fmodChannel->setVolume(value);
		FMODCheckError(result);
	}

	void AudioManager::SetChannelPitch(int id, float value)
	{
		// This call sets the pitch value on the channel. We can't modify 
		// the value, we can only set it.
		FMOD_RESULT result = m_ChannelList[id]->fmodChannel->setPitch(value);
		FMODCheckError(result);
	}

	void AudioManager::SetChannelPan(int id, float value)
	{
		// This call sets the pan value on the channel. We can't modify 
		// the value, we can only set it.
		FMOD_RESULT result = m_ChannelList[id]->fmodChannel->setPan(value);
		FMODCheckError(result);
	}

	bool AudioManager::GetChannelPlaying(int id)
	{
		// Check if the channel id is currently playing or not.
		// If it is not playing we can do a cleanup
		bool isPlaying;
		FMOD_RESULT result = m_ChannelList[id]->fmodChannel->isPlaying(&isPlaying);
		return isPlaying;
	}

	void AudioManager::GetPlaybackPosition(int id, unsigned int& value)
	{
		// Use this call to get the playback position of a channel.
		FMOD_RESULT result = m_ChannelList[id]->fmodChannel->getPosition(&value, FMOD_TIMEUNIT_MS);
		FMODCheckError(result);
	}

	void AudioManager::PrintInfo() const
	{
		size_t size = m_ChannelList.size();

		for (size_t i = 0; i < m_ChannelList.size(); i++)
		{
			// You can fill this in.
		}
	}
} // end audio namespace