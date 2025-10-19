// Controlador principal do emulador Chip-8
// Integra todos os módulos e gerencia o ciclo de execução

#include "../include/chip8.h"
#include <iostream>

// Construtor: inicializa ponteiros e flags
Chip8::Chip8() : display(nullptr), cpu(nullptr), scale(Config::Display::DEFAULT_SCALE), clock_speed(Config::CPU::DEFAULT_CLOCK_SPEED), initialized(false) {}

Chip8::~Chip8() {
    if (cpu) delete cpu;
    if (display) delete display;
}

// Inicializa todos os módulos
void Chip8::initialize(int scale, int clock) {
    if (initialized) return;
    this->scale = scale;
    this->clock_speed = clock;
    display = new Display(scale);
    cpu = new CPU(memory, *display, input, audio);
    cpu->set_clock_speed(clock);
    initialized = true;
}

// Carrega uma ROM no endereço especificado
bool Chip8::load_rom(const std::string& path, uint16_t load_address) {
    if (!memory.load_rom(path, load_address)) {
        std::cerr << "[Chip8] ERRO: Falha ao carregar ROM: " << path << std::endl;
        return false;
    }
    cpu->reset();
    return true;
}

// Executa um ciclo de CPU
void Chip8::emulate_cycle() {
    if (initialized) cpu->emulate_cycle();
}

// Atualiza timers
void Chip8::update_timers() {
    if (initialized) cpu->update_timers();
}

// Processa eventos de teclado
void Chip8::handle_input(const SDL_Event& event) {
    input.handle_event(event);
}

// Atualiza o display
void Chip8::draw() {
    if (initialized) display->render();
}
