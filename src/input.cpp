// Módulo de entrada do Chip-8
// Gerencia o teclado hexadecimal e mapeia para teclas físicas

#include "../include/input.h"
#include <iostream>
#include <algorithm>

// Construtor: inicializa todas as teclas como não pressionadas
Input::Input() {
    std::fill(std::begin(keys), std::end(keys), false);
}

Input::~Input() {}

// Mapeia SDL_Keycode para índice CHIP-8
int Input::map_key(SDL_Keycode key) const {
    switch (key) {
        case SDLK_1: return 0x1;
        case SDLK_2: return 0x2;
        case SDLK_3: return 0x3;
        case SDLK_4: return 0xC;
        case SDLK_q: return 0x4;
        case SDLK_w: return 0x5;
        case SDLK_e: return 0x6;
        case SDLK_r: return 0xD;
        case SDLK_a: return 0x7;
        case SDLK_s: return 0x8;
        case SDLK_d: return 0x9;
        case SDLK_f: return 0xE;
        case SDLK_z: return 0xA;
        case SDLK_x: return 0x0;
        case SDLK_c: return 0xB;
        case SDLK_v: return 0xF;
        default: return -1;
    }
}

// Processa evento de teclado SDL
void Input::handle_event(const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
        int idx = map_key(e.key.keysym.sym);
        if (idx >= 0 && idx < 16) {
            keys[idx] = (e.type == SDL_KEYDOWN);
        }
    }
}

// Verifica se uma tecla CHIP-8 está pressionada
bool Input::is_pressed(uint8_t key) const {
    if (key < 16) return keys[key];
    std::cerr << "[Input] ERRO: Tecla inválida consultada: " << (int)key << std::endl;
    return false;
}

// Bloqueia até que o usuário pressione uma tecla válida
uint8_t Input::wait_for_key() {
    SDL_Event e;
    while (true) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                int idx = map_key(e.key.keysym.sym);
                if (idx >= 0 && idx < 16) {
                    keys[idx] = true;
                    return idx;
                }
            }
        }
        SDL_Delay(1); 
    }
}
