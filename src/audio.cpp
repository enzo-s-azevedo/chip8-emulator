// Módulo de áudio do Chip-8
// Simula o beep usando SDL2

#include "../include/audio.h"
#include <iostream>
#include <cstring>

// Parâmetros do beep
const int SAMPLE_RATE = 44100;
const int AMPLITUDE = 128;
const int FREQUENCY = 440; 
const int CHANNELS = 1;
const int BUFFER_SIZE = 1024;

// Gera o som do Chip-8 criando uma onda quadrada quando o Sound Timer está ativo
void Audio::audio_callback(void* userdata, Uint8* stream, int len) {
    static int phase = 0;
    for (int i = 0; i < len; ++i) {
        stream[i] = (phase < SAMPLE_RATE / (2 * FREQUENCY)) ? (AMPLITUDE * 2) : 0;
        phase = (phase + 1) % (SAMPLE_RATE / FREQUENCY);
    }
}

// Inicializa o sistema de áudio e dispositivo
Audio::Audio() : device(0), is_playing(false) {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "[Audio] ERRO: Não foi possível inicializar SDL2 Audio: " << SDL_GetError() << std::endl;
        throw std::runtime_error("Falha ao inicializar SDL2 Audio");
    }
    SDL_AudioSpec want, have;
    SDL_zero(want);
    want.freq = SAMPLE_RATE;
    want.format = AUDIO_U8;
    want.channels = CHANNELS;
    want.samples = BUFFER_SIZE;
    want.callback = audio_callback;
    want.userdata = nullptr;
    device = SDL_OpenAudioDevice(nullptr, 0, &want, &have, 0);
    if (device == 0) {
        std::cerr << "[Audio] ERRO: Não foi possível abrir dispositivo de áudio: " << SDL_GetError() << std::endl;
        throw std::runtime_error("Falha ao abrir dispositivo de áudio");
    }
}

// Libera recursos de áudio
Audio::~Audio() {
    stop_beep();
    if (device) SDL_CloseAudioDevice(device);
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

// Inicia a reprodução do beep
void Audio::start_beep() {
    if (!is_playing && device) {
        SDL_PauseAudioDevice(device, 0);
        is_playing = true;
    }
}

// Interrompe o beep
void Audio::stop_beep() {
    if (is_playing && device) {
        SDL_PauseAudioDevice(device, 1);
        is_playing = false;
    }
}
