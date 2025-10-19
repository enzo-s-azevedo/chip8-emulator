// Controlador principal do emulador Chip-8
// Integra todos os módulos e gerencia o ciclo de execução

#pragma once
#include "config.h"
#include "memory.h"
#include "display.h"
#include "input.h"
#include "audio.h"
#include "cpu.h"
#include <string>
#include <SDL2/SDL.h>

class Chip8 {
public:
    Chip8();
    ~Chip8();

    // Inicializa todos os módulos
    void initialize(int scale = Config::Display::DEFAULT_SCALE, int clock = Config::CPU::DEFAULT_CLOCK_SPEED);

    // Carrega uma ROM no endereço especificado
    bool load_rom(const std::string& path, uint16_t load_address = Config::Memory::PROGRAM_START);

    // Executa um ciclo de CPU
    void emulate_cycle();

    // Atualiza timers 
    void update_timers();

    // Processa eventos de teclado
    void handle_input(const SDL_Event& event);

    // Atualiza o display
    void draw();

private:
    Memory memory;
    Display* display;
    Input input;
    Audio audio;
    CPU* cpu;
    int scale;
    int clock_speed;
    bool initialized;
};
