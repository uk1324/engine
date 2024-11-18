#include "Audio.hpp"
#include <engine/Audio/AudioErrorHandling.hpp>

static ALCdevice* device;
static ALCcontext* context;

void Audio::init() {
    ALCdevice* defaultDevice = alcOpenDevice(nullptr);
    if (defaultDevice == nullptr) {
        handleAudioError();
        return;
    }
    device = defaultDevice;

    context = alcCreateContext(device, nullptr);
    if (context == nullptr) {
        handleAudioError();
        return;
    }

    const auto result = alcMakeContextCurrent(context);
    if (!result) {
        handleAudioError();
        return;
    }
}

void Audio::deinit()
{
}

ALCdevice* Audio::device;
ALCcontext* Audio::context;