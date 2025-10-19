Compilação do emulador CHIP-8

Pré-requisitos
- Compilador C++17 (g++ ou clang++)
- SDL2
- Make

Linux (Debian/Ubuntu)
- Instale as dependências:
	- sudo apt update
	- sudo apt install build-essential libsdl2-dev
- Certifique-se de estar na pasta raiz do projeto
- Compile:
	- make
Resultado
- O executável será gerado em build/chip8-emulator

Windows (MSYS2/MinGW64)
- Instale o MSYS2: https://www.msys2.org/
- No terminal MINGW64, instale:
	- pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-SDL2 make
- Certifique-se de estar na pasta raiz do projeto    
- Compile:
	- make
Resultado
- O executável será gerado em build/chip8-emulator.exe

Alvos do Makefile
- make            -> compila para build/chip8-emulator(.exe)
- make clean      -> remove objetos e executável de build/
- make rebuild    -> limpa e recompila
- make help       -> mostra comandos disponíveis
- make print-sdl2 -> mostra flags detectadas do SDL2

