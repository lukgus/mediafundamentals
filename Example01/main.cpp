#include <fmod.hpp>
#include <iostream>	// printf
#include <conio.h>	// _kbhit, _getch

void PlaySound(const char* soundfile)
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
		// Cleanup
		result = system->close();
		if (result != FMOD_OK)
		{
			printf("Failed to close system!\n");
		}
		return;
	}

	result = system->createSound(soundfile, FMOD_DEFAULT, 0, &sound);
	if (result != FMOD_OK)
	{
		printf("Failed to load the sound file: soundfile.wav\n");
		// Cleanup
		result = sound->release();
		if (result != FMOD_OK)
		{
			printf("Failed to release sound!\n");
		}

		result = system->close();
		if (result != FMOD_OK)
		{
			printf("Failed to close system!\n");
		}
		return;
	}

	result = system->playSound(sound, 0, false, &channel);
	if (result != FMOD_OK)
	{
		printf("Failed to play the sound!\n");
		// Cleanup
		result = sound->release();
		if (result != FMOD_OK)
		{
			printf("Failed to release sound!\n");
		}

		result = system->close();
		if (result != FMOD_OK)
		{
			printf("Failed to close system!\n");
		}

		result = system->release();
		if (result != FMOD_OK)
		{
			printf("Failed to release system!\n");
		}
		return;
	}

	printf("Sound playing, press ESC to quit . . .");
	while (true)
	{
		// Let FMOD update the audio buffer with the sounds that
		// are playing. This also does a lot more behind the 
		// scenes
		result = system->update();
		if (result != FMOD_OK)
		{
			printf("Failed to update!\n");
		}

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

	// Cleanup
	result = sound->release();
	if (result != FMOD_OK)
	{
		printf("Failed to release sound!\n");
	}

	result = system->close();
	if (result != FMOD_OK)
	{
		printf("Failed to close system!\n");
	}

	result = system->release();
	if (result != FMOD_OK)
	{
		printf("Failed to release system!\n");
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
	PlaySound("audio/jaguar.wav");
	PlaySound("audio/singing.wav");
	PlaySound("audio/swish.wav");

	// 0 means successful, anything else is typically an "error"
	return 0;
}