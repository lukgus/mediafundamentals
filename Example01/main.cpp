#include "SoundManager.h"
#include <iostream>
#include <conio.h>

SoundManager g_SoundManager;

void PlaySound(const char* soundfile)
{
	g_SoundManager.LoadSound(soundfile);
	g_SoundManager.PlaySound();

	printf("Sound playing, press ESC to quit . . .");
	while (true)
	{
		g_SoundManager.Update();

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
	g_SoundManager.Initialize();

	PlaySound("audio/jaguar.wav");
	PlaySound("audio/singing.wav");
	PlaySound("audio/swish.wav");

	g_SoundManager.Destroy();
	// 0 means successful, anything else is typically an "error"
	return 0;
}