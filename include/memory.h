// Módulo de gerenciamento de memória do Chip-8
// Gerencia os 4KB de RAM, carrega ROMs e inicializa sprites hexadecimais

#pragma once
#include <cstdint>
#include <string>
#include <array>
#include "config.h"

class Memory {
public:
    static constexpr uint16_t MEMORY_SIZE = Config::Memory::SIZE;
    static constexpr uint16_t PROGRAM_START = Config::Memory::PROGRAM_START;
    static constexpr uint16_t FONT_START = Config::Memory::FONT_START;
    static constexpr uint16_t FONT_SIZE = Config::Memory::FONT_SIZE;
    static constexpr uint16_t RESERVED_END = Config::Memory::RESERVED_END;

    // Inicializa a memória zerada e carrega os sprites hexadecimais
    Memory();
    
    ~Memory();

    // Limpa toda a memória e recarrega os sprites
    void clear();

    // Lê um byte da memória no endereço especificado
    uint8_t read(uint16_t address) const;

    // Escreve um byte na memória no endereço especificado
    void write(uint16_t address, uint8_t value);

    // Carrega uma ROM do arquivo para a memória
    bool load_rom(const std::string& rom_path, uint16_t load_address = PROGRAM_START);

    // Retorna o endereço inicial de um sprite hexadecimal
    uint16_t get_font_address(uint8_t digit) const;


private:
    // Array de 4KB representando a memória RAM do Chip-8
    std::array<uint8_t, MEMORY_SIZE> ram;

    // Carrega os sprites hexadecimais na área reservada da memória
    void load_fonts();

    // Verifica se um endereço está dentro dos limites válidos
    bool is_valid_address(uint16_t address) const;
};

