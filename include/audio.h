// Módulo de áudio do Chip-8
// Simula o beep usando SDL2

#pragma once
#include <SDL2/SDL.h>

class Audio {
public:
    Audio();
    ~Audio();

    // Inicia a reprodução do beep
    void start_beep();

    // Interrompe o beep
    void stop_beep();

private:
    SDL_AudioDeviceID device;
    bool is_playing;
    static void audio_callback(void* userdata, Uint8* stream, int len);
};
