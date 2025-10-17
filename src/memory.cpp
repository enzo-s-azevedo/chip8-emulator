// Implementação do módulo de memória do Chip-8
// Responsável por gerenciar os 4KB de RAM, incluindo leitura, escrita e carregamento de ROMs

#include "../include/memory.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

static const uint8_t CHIP8_FONTS[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,
    0x20, 0x60, 0x20, 0x20, 0x70,
    0xF0, 0x10, 0xF0, 0x80, 0xF0,
    0xF0, 0x10, 0xF0, 0x10, 0xF0,
    0x90, 0x90, 0xF0, 0x10, 0x10,
    0xF0, 0x80, 0xF0, 0x10, 0xF0,
    0xF0, 0x80, 0xF0, 0x90, 0xF0,
    0xF0, 0x10, 0x20, 0x40, 0x40,              
    0xF0, 0x90, 0xF0, 0x90, 0xF0,
    0xF0, 0x90, 0xF0, 0x10, 0xF0,
    0xF0, 0x90, 0xF0, 0x90, 0x90,
    0xE0, 0x90, 0xE0, 0x90, 0xE0,
    0xF0, 0x80, 0x80, 0x80, 0xF0,
    0xE0, 0x90, 0x90, 0x90, 0xE0,
    0xF0, 0x80, 0xF0, 0x80, 0xF0,
    0xF0, 0x80, 0xF0, 0x80, 0x80
};

// Construtor: inicializa a memória e carrega os sprites
Memory::Memory() {
    clear();
}

// Destrutor
Memory::~Memory() {}

// Limpa toda a memória e recarrega os sprites
void Memory::clear() {
    ram.fill(0);
    load_fonts();
    
    std::cout << "[Memory] Memória limpa e sprites carregados." << std::endl;
}

// Carrega os sprites hexadecimais na área reservada da memória 
void Memory::load_fonts() {
    for (size_t i = 0; i < FONT_SIZE; ++i) {
        ram[FONT_START + i] = CHIP8_FONTS[i];
    }
}

// Verifica se um endereço está dentro dos limites válidos da memória
bool Memory::is_valid_address(uint16_t address) const {
    return address < MEMORY_SIZE;
}

// Lê um byte da memória no endereço especificado
uint8_t Memory::read(uint16_t address) const {
    if (!is_valid_address(address)) {
        std::cerr << "[Memory] ERRO: Tentativa de leitura em endereço inválido: 0x" 
                  << std::hex << address << std::dec << std::endl;
        throw std::out_of_range("Endereço de memória fora dos limites");
    }
    
    return ram[address];
}

// Escreve um byte na memória no endereço especificado
void Memory::write(uint16_t address, uint8_t value) {
    if (!is_valid_address(address)) {
        std::cerr << "[Memory] ERRO: Tentativa de escrita em endereço inválido: 0x" 
                  << std::hex << address << std::dec << std::endl;
        throw std::out_of_range("Endereço de memória fora dos limites");
    }
    if (address < FONT_START + FONT_SIZE) {
        std::cerr << "[Memory] AVISO: Escrita na área de sprites (0x" 
                  << std::hex << address << std::dec << ")" << std::endl;
    }
    
    ram[address] = value;
}

// Carrega uma ROM do arquivo para a memória
bool Memory::load_rom(const std::string& rom_path, uint16_t load_address) {
    // Verifica se o endereço de carregamento é válido
    if (!is_valid_address(load_address)) {
        std::cerr << "[Memory] ERRO: Endereço de carregamento inválido: 0x" 
                  << std::hex << load_address << std::dec << std::endl;
        return false;
    }
    
    if (load_address <= RESERVED_END) {
        std::cerr << "[Memory] AVISO: Carregando ROM na área reservada (0x" 
                  << std::hex << load_address << std::dec << ")" << std::endl;
    }
    
    std::ifstream file(rom_path, std::ios::binary | std::ios::ate);
    
    if (!file.is_open()) {
        std::cerr << "[Memory] ERRO: Não foi possível abrir a ROM: " << rom_path << std::endl;
        return false;
    }
    
    // Obtém o tamanho do arquivo
    std::streamsize file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    if (file_size <= 0) {
        std::cerr << "[Memory] ERRO: A ROM está vazia: " << rom_path << std::endl;
        file.close();
        return false;
    }
    
    // Verifica se a ROM cabe na memória disponível
    if (load_address + file_size > MEMORY_SIZE) {
        std::cerr << "[Memory] ERRO: A ROM é muito grande para caber na memória." << std::endl;
        std::cerr << "           Tamanho: " << file_size << " bytes" << std::endl;
        std::cerr << "           Espaço disponível: " << (MEMORY_SIZE - load_address) << " bytes" << std::endl;
        file.close();
        return false;
    }
    
    // Lê o conteúdo do arquivo diretamente para a memória
    if (!file.read(reinterpret_cast<char*>(&ram[load_address]), file_size)) {
        std::cerr << "[Memory] ERRO: Falha ao ler o conteúdo da ROM." << std::endl;
        file.close();
        return false;
    }
    
    file.close();
    
    // Mensagem de sucesso
    std::cout << "[Memory] ROM carregada com sucesso!" << std::endl;
    std::cout << "         Arquivo: " << rom_path << std::endl;
    std::cout << "         Tamanho: " << file_size << " bytes" << std::endl;
    std::cout << "         Endereço: 0x" << std::hex << load_address 
              << " - 0x" << (load_address + file_size - 1) << std::dec << std::endl;
    
    return true;
}

// Obtém o endereço inicial de um sprite hexadecimal específico
uint16_t Memory::get_font_address(uint8_t digit) const {
    // Cada sprite tem 5 bytes, então o endereço é: FONT_START + (digit * 5)
    digit = digit & 0x0F; // Limita o dígito a 0x0-0xF
    return FONT_START + (digit * 5);
}

