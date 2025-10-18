// Módulo de entrada do Chip-8
// Gerencia o teclado hexadecimal (16 teclas) e mapeia para teclas físicas

#pragma once
#include <cstdint>
#include <SDL2/SDL.h>

class Input {
public:
    Input();
    ~Input();

    // Processa evento de teclado SDL
    void handle_event(const SDL_Event& e);

    // Verifica se uma tecla CHIP-8 está pressionada
    bool is_pressed(uint8_t key) const;

    // Bloqueia até que o usuário pressione uma tecla válida
    uint8_t wait_for_key();

private:
    bool keys[16]; // Estado das teclas (true = pressionada)

    // Converte a tecla pressionada para o índice correspondente no teclado do Chip-8
    int map_key(SDL_Keycode key) const;
};
