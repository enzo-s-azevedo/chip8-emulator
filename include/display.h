// Módulo de saída gráfica do Chip-8
// Responsável por renderizar a tela 64x32 usando SDL2

#pragma once
#include <cstdint>
#include <vector>
#include <SDL2/SDL.h>
#include "config.h"

class Display {
public:
    static constexpr int WIDTH = Config::Display::WIDTH;
    static constexpr int HEIGHT = Config::Display::HEIGHT;

    // Cria o display, inicializando SDL e a janela
    Display(int scale = Config::Display::DEFAULT_SCALE);
    ~Display();

    // Limpa a tela
    void clear();

    // Desenha um sprite na tela na posição (x, y)
    bool draw_sprite(uint8_t x, uint8_t y, const uint8_t* sprite, uint8_t n);

    // Atualiza a janela SDL com o estado atual dos pixels
    void render();


private:
    int scale; // Fator de escala
    std::vector<uint8_t> pixels; // Framebuffer 64x32
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    // Atualiza a textura SDL com o framebuffer
    void update_texture();
};
