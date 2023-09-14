#include <fmod.hpp>
#include <iostream>	// printf
#include <conio.h>	// _kbhit, _getch

void PlaySound()
{
	FMOD::System* system = nullptr;
	FMOD::Sound* sound = nullptr;
	FMOD::Channel* channel = nullptr;
	FMOD_RESULT result;
	
	result = FMOD::System_Create(&system);
	if (result != FMOD_OK)
	{
		printf("Failed to create the FMOD System!\n");
		return;
	}

	result = system->init(1, FMOD_INIT_NORMAL, nullptr);
	if (result != FMOD_OK)
	{
		printf("Failed to initialize the system!\n");
		// TODO: FMOD Cleanup system
		// 'delete system' or see if there is a FMOD::System_Destroy(..)
		return;
	}

	result = system->createSound("soundfile.wav", FMOD_DEFAULT, 0, &sound);
	if (result != FMOD_OK)
	{
		printf("Failed to load the sound file: soundfile.wav\n");
		// TODO: FMOD Cleanup system
		// 'delete system' or see if there is a FMOD::System_Destroy(..)
		return;
	}

	result = system->playSound(sound, 0, false, &channel);
	if (result != FMOD_OK)
	{
		printf("Failed to play the sound!\n");
		// TODO: FMOD Cleanup system
		// 'delete system' or see if there is a FMOD::System_Destroy(..)
		// 'delete sound' or see if there is a FMOD::Sound_Destroy(..)
		return;
	}

	printf("Sound playing, press ESC to quit . . .");
	while (true)
	{
		// Wait here until user presses ESCAPE
		// std::cin >>  // This is a blocking call

		// We need a nonblocking solution
		// conio.h	_kbhit() _getch()
		if (_kbhit())
		{
			int key = _getch();
			if (key == 27/*ESCAPE*/)
			{
				break;
			}
		}
	}
}


// Main entry point to our program
// argc is the number of arguments
// argv is the arguments
// 
// Examples of input:
// Example01.exe (1, "")
// Example01.exe with some parameters (4, {"Example01", "with", "some", "Parameters"})
int main(int argc, char** argv)
{
	PlaySound();

	// 0 means successful, anything else is typically an "error"
	return 0;
}