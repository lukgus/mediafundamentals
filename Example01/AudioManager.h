#pragma once

#include <fmod.hpp>
#include "AudioUtils.h"
#include "common.h"

#include <map>
#include <vector>

namespace audio
{
	// Move this
	struct Audio
	{
		const char* name;
		FMOD::Sound* Audio;
	};

	struct Channel
	{
		const char* name;
		FMOD::Channel* fmodChannel;
		float volume;
		float pitch;
		float pan;
		bool playing;
	};

	// This can be a singleton, but we are not focusing on 
	// design patterns at the moment.
	class AudioManager
	{
	public:
		// Constructor
		AudioManager();

		// Destructor (Gets called on delete)
		~AudioManager();

		// System calls
		void Initialize();
		void Update();
		void Destroy();

		// Loading Audio 
		void LoadAudioStream(const char* source);
		void LoadAudio(const char* file);

		// Play audio
		int PlayAudio(const char* AudioName);

		// Modifying Channel Values
		void SetChannelVolume(int id, float value);
		void SetChannelPitch(int id, float value);
		void SetChannelPan(int id, float value);

		// Retrieving information
		bool GetChannelPlaying(int id);
		void GetPlaybackPosition(int id, unsigned int& value);


		// void SetChannelBLAHBLAH(int id, float value);
		// Also add GetChannelBLABLA(int id, float &value);


		void PrintInfo() const;

	private:

		bool m_Initialized = false;


		std::map<const char*, Audio*> m_AudioMap;	// This is our cache
		std::vector<Channel*> m_ChannelList;		// Maintaint a list of channels 

		int m_NextChannelId = 0;					// The next channel Id that we can return

		FMOD::System* m_System = nullptr;
	};
}