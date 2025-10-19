// Unidade Central de Processamento do Chip-8
// Simula o processador virtual: fetch, decode, execute

#pragma once
#include <cstdint>
#include <array>
#include "config.h"
#include "memory.h"
#include "display.h"
#include "input.h"
#include "audio.h"

class CPU {
public:
    CPU(Memory& memory, Display& display, Input& input, Audio& audio);
    ~CPU();

    // Reinicia a CPU para o estado inicial
    void reset();

    // Executa um ciclo de instrução (fetch-decode-execute)
    void emulate_cycle();

    // Atualiza os timers 
    void update_timers();

    // Define a velocidade do clock
    void set_clock_speed(int hz);

private:
    // Registradores
    std::array<uint8_t, 16> V;  // V0-VF
    uint16_t I;                  // Registrador de endereço
    uint16_t PC;                 // Contador de programa
    uint8_t SP;                  // Ponteiro de pilha

    // Pilha
    std::array<uint16_t, 16> stack;

    // Timers
    uint8_t delay_timer;
    uint8_t sound_timer;

    // Referências aos módulos
    Memory& memory;
    Display& display;
    Input& input;
    Audio& audio;

    // Instruções por segundo
    int clock_speed;

    // Decodifica e executa um opcode
    void execute_opcode(uint16_t opcode);

    // Funções auxiliares para grupos de opcodes
    void execute_0xxx(uint16_t opcode);
    void execute_8xxx(uint16_t opcode);
    void execute_Exxx(uint16_t opcode);
    void execute_Fxxx(uint16_t opcode);
};
