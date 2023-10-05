#include "AudioDemo.h"
#include <conio.h> // _kbhit & _getch
void AudioDemo::Prepare()
{
	// Load sound.txt
	// foreach line, get sound file
	//	 m_AudioManager->LoadAudio(soundFile)

	m_AudioManager = new audio::AudioManager();
	m_AudioManager->Initialize();
	m_AudioManager->LoadAudio("audio/jaguar.wav");
	m_AudioManager->LoadAudio("audio/singing.wav");
	m_AudioManager->LoadAudio("audio/swish.wav");
	m_AudioManager->LoadAudio("audio/awesomeness.wav");

	printf("Press [1,2,3,4] to play a sound.\n");
}

void AudioDemo::Shutdown()
{
	m_AudioManager->Destroy();
	delete m_AudioManager;
}

void AudioDemo::RunDemo()
{
	bool m_Running = true;
	int channel = 0;
	unsigned int position = 0;
	while (m_Running)
	{
		m_AudioManager->Update();
		//PrintInfo();
		if (_kbhit())
		{
			int key = _getch();
			if (key == '1')
			{
				channel = m_AudioManager->PlayAudio("audio/jaguar.wav");
			}
			else if (key == '2')
			{
				channel = m_AudioManager->PlayAudio("audio/singing.wav");
			}
			else if (key == '3')
			{
				m_AudioManager->PlayAudio("audio/swish.wav");
			}
			else if (key == '4')
			{
				m_AudioManager->PlayAudio("audio/awesomeness.wav");
			}
			if (key == 27/*ESCAPE*/)
			{
				StopDemo();
			}
		}

		if (m_AudioManager->GetChannelPlaying(channel))
		{
			m_AudioManager->GetPlaybackPosition(channel, position);
			printf("Playback: %d\n", position);
		}
	}
}

void AudioDemo::PrintInfo() const
{
	m_AudioManager->PrintInfo();
}

void AudioDemo::StopDemo()
{
	m_Running = false;
}