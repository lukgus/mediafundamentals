#pragma once

#include <fmod.h>
#include "common.h"

gdpAudioNamespace_Begin

	void CheckError(FMOD_RESULT result, const char* file, int line);
#define FMODCheckError(result) CheckError(result, __FILE__, __LINE__)

gdpAudioNamespace_End
