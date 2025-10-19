# Compilador e flags
CXX      ?= g++
CXXFLAGS  = -std=c++17 -Wall -Wextra -Wpedantic -O2
TARGET    = chip8-emulator

# Diretórios
SRC_DIR     = src
INCLUDE_DIR = include

# Detectar sistema operacional
UNAME_S := $(shell uname -s 2>/dev/null || echo "Windows")

# Configurações por SO
ifeq ($(findstring MINGW,$(UNAME_S)),MINGW)
    INCLUDES   = -I$(INCLUDE_DIR) -DSDL_MAIN_HANDLED -IC:/msys64/mingw64/include/SDL2
    LIBS       = -LC:/msys64/mingw64/lib -lmingw32 -lSDL2main -lSDL2 -mwindows
    TARGET_EXT = .exe
    RM         = rm -f
else ifeq ($(UNAME_S),Linux)
    INCLUDES   = -I$(INCLUDE_DIR) -DSDL_MAIN_HANDLED $(shell sdl2-config --cflags 2>/dev/null)
    LIBS       = $(shell sdl2-config --libs 2>/dev/null)
    TARGET_EXT =
    RM         = rm -f
else
    # Fallback Windows CMD (se não for MINGW)
    INCLUDES   = -I$(INCLUDE_DIR) -DSDL_MAIN_HANDLED -IC:/msys64/mingw64/include/SDL2
    LIBS       = -LC:/msys64/mingw64/lib -lmingw32 -lSDL2main -lSDL2 -mwindows
    TARGET_EXT = .exe
    RM         = del /f /q
endif

# Diretório de saída e executável
BUILD_DIR = build
BIN       = $(BUILD_DIR)/$(TARGET)$(TARGET_EXT)

# Arquivos fonte / objetos
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Alvos principais
.PHONY: all clean run rebuild help print-sdl2

all: $(BIN)

# Linkagem

# Criar build/ se não existir
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Linkagem
$(BIN): $(OBJECTS) | $(BUILD_DIR)
	@echo "Linkando $(BIN)..."
	$(CXX) $(OBJECTS) -o $@ $(LIBS)
	@echo "Build successful! Executable: $(BIN)"

# Compilação dos objetos
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@echo "Compilando $<..."
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Execução
ROM       ?= roms/PONG
SCALE     ?= 10
CLOCK     ?= 500
LOAD      ?= 0x200

run: $(BIN)
	@echo "Executando: $(BIN) --rom $(ROM) --scale $(SCALE) --clock $(CLOCK) --loadaddr $(LOAD)"
	$(BIN) --rom $(ROM) --scale $(SCALE) --clock $(CLOCK) --loadaddr $(LOAD)

# Mostrar flags SDL2
print-sdl2:
	@echo SDL2_CFLAGS="$(SDL2_CFLAGS)"
	@echo SDL2_LIBS="$(SDL2_LIBS)"

# Limpeza
clean:
	@echo "Limpando arquivos de build..."
	$(RM) $(OBJECTS) $(BIN) 2>/dev/null || true
	$(RM) $(BUILD_DIR)/* 2>/dev/null || true
	@echo "Limpeza concluída!"

# Recompilar do zero
rebuild:
	$(RM) $(OBJECTS) $(BIN) 2>/dev/null || true
	$(MAKE) -B all

# Ajuda
help:
	@echo "====================================="
	@echo "CHIP-8 Emulator - Comandos Makefile"
	@echo "====================================="
	@echo ""
	@echo "Comandos:"
	@echo "  make           - Compila o emulador"
	@echo "  make clean     - Remove arquivos de build"
	@echo "  make run       - Executa com ROM (use ROM=...)"
	@echo "  make rebuild   - Limpa e recompila"
	@echo "  make print-sdl2- Mostra flags do SDL2"
	@echo "  make help      - Mostra esta ajuda"
	@echo ""
	@echo "Uso:"
	@echo "  ./$(BIN) --rom roms/<ROM> [--scale N] [--clock Hz] [--loadaddr 0xHEX]"
	@echo ""

# Default target
.DEFAULT_GOAL := all
