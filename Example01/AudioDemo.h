#pragma once

#include "AudioManager.h"

class AudioDemo
{
public:
	AudioDemo() = default;
	~AudioDemo() { }

	void Prepare();
	void Shutdown();

	void RunDemo();

private:
	void StopDemo();

	inline void PrintInfo() const;

	gdpAudioNamespace AudioManager* m_AudioManager;
	bool m_Running;
};