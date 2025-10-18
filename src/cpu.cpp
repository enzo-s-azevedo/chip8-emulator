// Unidade Central de Processamento do Chip-8
// Implementa o ciclo fetch-decode-execute e todos os opcodes

#include "../include/cpu.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

// Construtor: inicializa CPU e seus componentes
CPU::CPU(Memory& memory, Display& display, Input& input, Audio& audio)
    : memory(memory), display(display), input(input), audio(audio), clock_speed(500) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    reset();
}

CPU::~CPU() {}

// Reinicia a CPU para o estado inicial
void CPU::reset() {
    V.fill(0);
    I = 0;
    PC = Memory::PROGRAM_START;
    SP = 0;
    stack.fill(0);
    delay_timer = 0;
    sound_timer = 0;
    display.clear();
}

// Define a velocidade do clock
void CPU::set_clock_speed(int hz) {
    if (hz > 0) clock_speed = hz;
}

// Executa um ciclo de instrução
void CPU::emulate_cycle() {
    // Fetch
    uint16_t opcode = (memory.read(PC) << 8) | memory.read(PC + 1);
    PC += 2;

    // Decode & Execute
    execute_opcode(opcode);
}

// Atualiza os timers
void CPU::update_timers() {
    if (delay_timer > 0) --delay_timer;
    if (sound_timer > 0) {
        if (sound_timer == 1) audio.start_beep();
        --sound_timer;
    } else {
        audio.stop_beep();
    }
}

// Decodifica e executa um opcode
void CPU::execute_opcode(uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint8_t n = opcode & 0x000F;
    uint8_t kk = opcode & 0x00FF;
    uint16_t nnn = opcode & 0x0FFF;

    switch (opcode & 0xF000) {
        case 0x0000: execute_0xxx(opcode); break;
        case 0x1000: PC = nnn; break; // 1NNN: JP addr
        case 0x2000: // 2NNN: CALL addr
            if (SP >= 16) {
                std::cerr << "[CPU] ERRO: Stack overflow!" << std::endl;
                return;
            }
            stack[SP++] = PC;
            PC = nnn;
            break;
        case 0x3000: if (V[x] == kk) PC += 2; break; // 3XKK: SE Vx, byte
        case 0x4000: if (V[x] != kk) PC += 2; break; // 4XKK: SNE Vx, byte
        case 0x5000: if (V[x] == V[y]) PC += 2; break; // 5XY0: SE Vx, Vy
        case 0x6000: V[x] = kk; break; // 6XKK: LD Vx, byte
        case 0x7000: V[x] += kk; break; // 7XKK: ADD Vx, byte
        case 0x8000: execute_8xxx(opcode); break;
        case 0x9000: if (V[x] != V[y]) PC += 2; break; // 9XY0: SNE Vx, Vy
        case 0xA000: I = nnn; break; // ANNN: LD I, addr
        case 0xB000: PC = nnn + V[0]; break; // BNNN: JP V0, addr
        case 0xC000: V[x] = (std::rand() % 256) & kk; break; // CXKK: RND Vx, byte
        case 0xD000: { // DXYN: DRW Vx, Vy, nibble
            uint8_t* sprite = const_cast<uint8_t*>(&memory.read(I));
            bool collision = display.draw_sprite(V[x], V[y], sprite, n);
            V[0xF] = collision ? 1 : 0;
            break;
        }
        case 0xE000: execute_Exxx(opcode); break;
        case 0xF000: execute_Fxxx(opcode); break;
        default:
            std::cerr << "[CPU] ERRO: Opcode desconhecido: 0x" << std::hex << opcode << std::dec << std::endl;
            break;
    }
}

// Executa opcodes 0xxx (operações de controle de tela e retorno de sub-rotina)
void CPU::execute_0xxx(uint16_t opcode) {
    switch (opcode) {
        case 0x00E0: display.clear(); break; // 00E0: CLS
        case 0x00EE: // 00EE: RET
            if (SP == 0) {
                std::cerr << "[CPU] ERRO: Stack underflow!" << std::endl;
                return;
            }
            PC = stack[--SP];
            break;
        default:
            std::cerr << "[CPU] ERRO: Opcode 0xxx desconhecido: 0x" << std::hex << opcode << std::dec << std::endl;
            break;
    }
}

// Executa opcodes 8xxx (operações aritméticas e lógicas entre registradores
void CPU::execute_8xxx(uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    switch (opcode & 0x000F) {
        case 0x0: V[x] = V[y]; break; // 8XY0: LD Vx, Vy
        case 0x1: V[x] |= V[y]; break; // 8XY1: OR Vx, Vy
        case 0x2: V[x] &= V[y]; break; // 8XY2: AND Vx, Vy
        case 0x3: V[x] ^= V[y]; break; // 8XY3: XOR Vx, Vy
        case 0x4: { // 8XY4: ADD Vx, Vy
            uint16_t sum = V[x] + V[y];
            V[0xF] = (sum > 0xFF) ? 1 : 0;
            V[x] = sum & 0xFF;
            break;
        }
        case 0x5: // 8XY5: SUB Vx, Vy
            V[0xF] = (V[x] > V[y]) ? 1 : 0;
            V[x] -= V[y];
            break;
        case 0x6: // 8XY6: SHR Vx
            V[0xF] = V[x] & 0x1;
            V[x] >>= 1;
            break;
        case 0x7: // 8XY7: SUBN Vx, Vy
            V[0xF] = (V[y] > V[x]) ? 1 : 0;
            V[x] = V[y] - V[x];
            break;
        case 0xE: // 8XYE: SHL Vx
            V[0xF] = (V[x] & 0x80) >> 7;
            V[x] <<= 1;
            break;
        default:
            std::cerr << "[CPU] ERRO: Opcode 8xxx desconhecido: 0x" << std::hex << opcode << std::dec << std::endl;
            break;
    }
}

// Executa opcodes Exxx (verificações de teclas pressionadas)
void CPU::execute_Exxx(uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;

    switch (opcode & 0x00FF) {
        case 0x9E: if (input.is_pressed(V[x])) PC += 2; break; // EX9E: SKP Vx
        case 0xA1: if (!input.is_pressed(V[x])) PC += 2; break; // EXA1: SKNP Vx
        default:
            std::cerr << "[CPU] ERRO: Opcode Exxx desconhecido: 0x" << std::hex << opcode << std::dec << std::endl;
            break;
    }
}

// Executa opcodes Fxxx (timers, sprites, decimal codificado em binário e operações de memória com registradores)
void CPU::execute_Fxxx(uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;

    switch (opcode & 0x00FF) {
        case 0x07: V[x] = delay_timer; break; // FX07: LD Vx, DT
        case 0x0A: V[x] = input.wait_for_key(); break; // FX0A: LD Vx, K
        case 0x15: delay_timer = V[x]; break; // FX15: LD DT, Vx
        case 0x18: sound_timer = V[x]; break; // FX18: LD ST, Vx
        case 0x1E: I += V[x]; break; // FX1E: ADD I, Vx
        case 0x29: I = memory.get_font_address(V[x]); break; // FX29: LD F, Vx
        case 0x33: // FX33: LD B, Vx (BCD)
            memory.write(I, V[x] / 100);
            memory.write(I + 1, (V[x] / 10) % 10);
            memory.write(I + 2, V[x] % 10);
            break;
        case 0x55: // FX55: LD [I], Vx
            for (int i = 0; i <= x; ++i) memory.write(I + i, V[i]);
            break;
        case 0x65: // FX65: LD Vx, [I]
            for (int i = 0; i <= x; ++i) V[i] = memory.read(I + i);
            break;
        default:
            std::cerr << "[CPU] ERRO: Opcode Fxxx desconhecido: 0x" << std::hex << opcode << std::dec << std::endl;
            break;
    }
}
