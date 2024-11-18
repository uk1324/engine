#pragma once

#include <openal-soft/include/AL/alc.h>
#include <vector>

struct Audio {
	static void init();
	static void deinit();

	static ALCdevice* device;
	static ALCcontext* context;
};