// Configurações globais do emulador Chip-8
// Centraliza todas as constantes usadas pelos módulos

#pragma once
#include <cstdint>

// Configurações de Display
namespace Config {
    namespace Display {
        constexpr int WIDTH = 64;              // Largura da tela em pixels
        constexpr int HEIGHT = 32;             // Altura da tela em pixels
        constexpr int DEFAULT_SCALE = 10;      // Fator de escala padrão 
    }

    // Configurações de Memória
    namespace Memory {
        constexpr uint16_t SIZE = 4096;        // 4KB de memória total
        constexpr uint16_t PROGRAM_START = 0x200;  // Endereço inicial padrão para ROMs
        constexpr uint16_t FONT_START = 0x000;     // Endereço inicial dos sprites
        constexpr uint16_t FONT_SIZE = 80;         // Tamanho total dos sprites (16 chars × 5 bytes)
        constexpr uint16_t RESERVED_END = 0x1FF;   // Fim da área reservada
    }

    // Configurações de Áudio
    namespace Audio {
        constexpr int SAMPLE_RATE = 44100;     // Taxa de amostragem (Hz)
        constexpr int AMPLITUDE = 128;         // Amplitude do som
        constexpr int FREQUENCY = 440;         // Frequência do beep (Hz)
        constexpr int CHANNELS = 1;            // Mono
        constexpr int BUFFER_SIZE = 1024;      // Tamanho do buffer de áudio
    }

    // Configurações de CPU
    namespace CPU {
        constexpr int DEFAULT_CLOCK_SPEED = 500;  // Velocidade padrão (Hz)
        constexpr int TIMER_FREQUENCY = 60;       // Frequência dos timers (Hz)
        constexpr int STACK_SIZE = 16;            // Tamanho da pilha
        constexpr int NUM_REGISTERS = 16;         // Número de registradores (V0-VF)
    }
}
