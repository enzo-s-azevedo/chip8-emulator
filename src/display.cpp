// Módulo de saída gráfica do Chip-8
// Implementa a tela 64x32 usando SDL2

#include "../include/display.h"
#include <cstring>
#include <iostream>

// Cria o display, inicializando SDL e a janela
Display::Display(int scale) : scale(scale), pixels(WIDTH * HEIGHT, 0), window(nullptr), renderer(nullptr), texture(nullptr) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "[Display] ERRO: Não foi possível inicializar SDL2: " << SDL_GetError() << std::endl;
        throw std::runtime_error("Falha ao inicializar SDL2");
    }
    window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              WIDTH * scale, HEIGHT * scale, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "[Display] ERRO: Não foi possível criar a janela SDL: " << SDL_GetError() << std::endl;
        SDL_Quit();
        throw std::runtime_error("Falha ao criar janela SDL");
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "[Display] ERRO: Não foi possível criar renderer SDL: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw std::runtime_error("Falha ao criar renderer SDL");
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                               WIDTH, HEIGHT);
    if (!texture) {
        std::cerr << "[Display] ERRO: Não foi possível criar textura SDL: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw std::runtime_error("Falha ao criar textura SDL");
    }
    clear();
}

// Libera recursos SDL
Display::~Display() {
    if (texture) SDL_DestroyTexture(texture);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

// Limpa a tela
void Display::clear() {
    std::fill(pixels.begin(), pixels.end(), 0);
    render();
}

// Desenha um sprite na tela na posição (x, y)
bool Display::draw_sprite(uint8_t x, uint8_t y, const uint8_t* sprite, uint8_t n) {
    bool collision = false;
    for (uint8_t row = 0; row < n; ++row) {
        uint8_t sprite_byte = sprite[row];
        for (uint8_t col = 0; col < 8; ++col) {
            if ((sprite_byte & (0x80 >> col)) != 0) {
                int px = (x + col) % WIDTH;
                int py = (y + row) % HEIGHT;
                int idx = py * WIDTH + px;
                if (pixels[idx]) collision = true;
                pixels[idx] ^= 1;
            }
        }
    }
    render();
    return collision;
}

// Atualiza a janela SDL com o estado atual dos pixels
void Display::render() {
    update_texture();
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

// Atualiza a textura SDL com o framebuffer
void Display::update_texture() {
    uint32_t buffer[WIDTH * HEIGHT];
    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        buffer[i] = pixels[i] ? 0xFFFFFFFF : 0xFF000000; // Branco ou preto
    }
    SDL_UpdateTexture(texture, nullptr, buffer, WIDTH * sizeof(uint32_t));
}

