#include "SoundManager.h"

#include <iostream>

SoundManager::SoundManager()
{
	// Usually do nothing that requires code execution
	// Declare initial values for everything
}

SoundManager::~SoundManager()
{
	// Typically just for notifying things that this is being destroyed.
}

void SoundManager::Initialize()
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

	result = m_System->init(1, FMOD_INIT_NORMAL, nullptr);
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

	m_Initialized = true;
}

void SoundManager::Destroy()
{
	if (!m_Initialized)
		return;

	FMOD_RESULT result;

	result = m_Sound->release();
	FMODCheckError(result);

	result = m_System->close();
	FMODCheckError(result);

	result = m_System->release();
	FMODCheckError(result);

	m_Initialized = false;
}

void SoundManager::LoadSound(const char* file)
{
	if (!m_Initialized)
		return;

	FMOD_RESULT result;

	result = m_System->createSound(file, FMOD_DEFAULT, 0, &m_Sound);
	if (result != FMOD_OK)
	{
		printf("Failed to load the sound file: soundfile.wav\n");
	}
}

void SoundManager::PlaySound()
{
	if (!m_Initialized)
		return;

	FMOD_RESULT result;
	result = m_System->playSound(m_Sound, 0, false, &m_Channel);
	if (result != FMOD_OK)
	{
		printf("Failed to play the sound!\n");
		// Cleanup
		result = m_Sound->release();
		FMODCheckError(result);

		result = m_System->close();
		FMODCheckError(result);

		result = m_System->release();
		FMODCheckError(result);
		return;
	}
}

void SoundManager::Update()
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
