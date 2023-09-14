#pragma once

#include <fmod.hpp>
#include "SoundUtils.h"

// This can be a singleton, but we are not focusing on 
// design patterns at the moment.
class SoundManager
{
public:
	// Constructor
	SoundManager();

	// Destructor (Gets called on delete)
	~SoundManager();

	void Initialize();
	void Destroy();

	void LoadSound(const char* file);
	void PlaySound();

	void Update();

private:
	bool m_Initialized = false;

	FMOD::System* m_System = nullptr;
	FMOD::Sound* m_Sound = nullptr;
	FMOD::Channel* m_Channel = nullptr;
	FMOD_RESULT m_Result;
};