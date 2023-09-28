#include "AudioDemo.h"

int main(int argc, char** argv)
{
	AudioDemo demo;
	demo.Prepare();
	demo.RunDemo();
	demo.Shutdown();
	return 0;
}