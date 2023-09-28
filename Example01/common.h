#pragma once



#define UseAudioNamespace
#ifdef UseAudioNamespace
#define gdpAudioNamespace_Begin	namespace audio {
#define gdpAudioNamespace_End	}
#define gdpAudioNamespace ::audio::
#else
#define gdpAudioNamespace_Begin
#define gdpAudioNamespace_End
#define gdpAudioNamespace
#endif