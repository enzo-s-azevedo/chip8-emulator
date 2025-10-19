// Ponto de entrada do emulador Chip-8
// Inicializa SDL, lê argumentos e coordena o loop principal

#include "../include/chip8.h"
#include "../include/config.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <filesystem>

static void print_usage(const char* exe) {
    std::cout << "Uso: " << exe << " --rom <arquivo> [--scale <valor>] [--clock <Hz>] [--loadaddr <hex>]" << std::endl;
    std::cout << "  --rom <arquivo>     Caminho da ROM .ch8" << std::endl;
    std::cout << "  --scale <valor>     Fator de escala da janela (padrão: " << Config::Display::DEFAULT_SCALE << ")" << std::endl;
    std::cout << "  --clock <Hz>        Clock da CPU em Hz (padrão: " << Config::CPU::DEFAULT_CLOCK_SPEED << ")" << std::endl;
    std::cout << "  --loadaddr <hex>    Endereço de carga em hex (padrão: 0x" << std::hex << Config::Memory::PROGRAM_START << std::dec << ")" << std::endl;
}

int main(int argc, char* argv[]) {
    std::string rom_path;
    int scale = Config::Display::DEFAULT_SCALE;
    int clock_hz = Config::CPU::DEFAULT_CLOCK_SPEED;
    uint16_t load_addr = Config::Memory::PROGRAM_START;

    // Parse simples de argumentos
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        auto need_value = [&](const char* name) {
            if (i + 1 >= argc) {
                std::cerr << "[main] ERRO: Falta valor para " << name << std::endl;
                print_usage(argv[0]);
                std::exit(1);
            }
        };

        if (arg == "--rom") {
            need_value("--rom");
            rom_path = argv[++i];
        } else if (arg == "--scale") {
            need_value("--scale");
            try {
                scale = std::stoi(argv[++i]);
                if (scale <= 0) throw std::invalid_argument("non-positive");
            } catch (...) {
                std::cerr << "[main] ERRO: Valor inválido para --scale" << std::endl;
                return 1;
            }
        } else if (arg == "--clock") {
            need_value("--clock");
            try {
                clock_hz = std::stoi(argv[++i]);
                if (clock_hz <= 0) throw std::invalid_argument("non-positive");
            } catch (...) {
                std::cerr << "[main] ERRO: Valor inválido para --clock" << std::endl;
                return 1;
            }
        } else if (arg == "--loadaddr") {
            need_value("--loadaddr");
            try {
                unsigned long v = std::stoul(argv[++i], nullptr, 0); // aceita 0x...
                if (v > 0xFFFF) throw std::out_of_range("range");
                load_addr = static_cast<uint16_t>(v);
            } catch (...) {
                std::cerr << "[main] ERRO: Valor inválido para --loadaddr (use ex.: 0x200)" << std::endl;
                return 1;
            }
        } else if (arg == "--help" || arg == "-h") {
            print_usage(argv[0]);
            return 0;
        } else {
            std::cerr << "[main] ERRO: Argumento desconhecido: " << arg << std::endl;
            print_usage(argv[0]);
            return 1;
        }
    }

    if (rom_path.empty()) {
        std::cerr << "[main] ERRO: Parâmetro --rom é obrigatório." << std::endl;
        print_usage(argv[0]);
        return 1;
    }
    if (!std::filesystem::exists(rom_path)) {
        std::cerr << "[main] ERRO: Arquivo ROM não encontrado: " << rom_path << std::endl;
        return 1;
    }

    // Inicializa SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) < 0) {
        std::cerr << "[main] ERRO: Falha ao inicializar SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    int exit_code = 0;
    {
        // Escopo para garantir destruição antes de SDL_Quit
        Chip8 chip8;
        try {
            chip8.initialize(scale, clock_hz);
        } catch (const std::exception& ex) {
            std::cerr << "[main] ERRO: Falha na inicialização do Chip8: " << ex.what() << std::endl;
            SDL_Quit();
            return 1;
        }

        if (!chip8.load_rom(rom_path, load_addr)) {
            SDL_Quit();
            return 1;
        }

        // Temporização
        using clock = std::chrono::steady_clock;
        auto next_cycle = clock::now();
        auto last_timer = clock::now();
        const auto cycle_period = std::chrono::nanoseconds(1'000'000'000ll / clock_hz);
        const auto timer_period = std::chrono::milliseconds(1000 / Config::CPU::TIMER_FREQUENCY);

        bool running = true;
        while (running) {
            // Eventos
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    running = false;
                } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
                chip8.handle_input(e);
            }

            // Ciclo de CPU conforme clock
            auto now = clock::now();
            if (now >= next_cycle) {
                chip8.emulate_cycle();
                next_cycle += cycle_period;
            } else {
                // Sleep para não ocupar 100% da CPU
                std::this_thread::sleep_for(std::chrono::microseconds(100));
            }

            // Timers a 60 Hz
            if (now - last_timer >= timer_period) {
                chip8.update_timers();
                last_timer = now;
            }

            // Atualiza a tela
            chip8.draw();
        }
    }

    SDL_Quit();
    return exit_code;
}
