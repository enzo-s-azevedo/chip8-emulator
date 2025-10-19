Uso do emulador

Sintaxe
- ./build/chip8-emulator --rom <ARQUIVO_ROM> [--scale <VALOR>] [--clock <Hz>] [--loadaddr <HEX>]

Parâmetros
- --rom <ARQUIVO_ROM>  Caminho da ROM (.ch8). Obrigatório.
- --scale <VALOR>      Fator de escala da janela. Padrão: 10.
- --clock <Hz>         Clock da CPU em Hz. Padrão: 500.
- --loadaddr <HEX>     Endereço de carga (ex.: 0x200). Padrão: 0x200.
- --help               Mostra ajuda.

Exemplos
- Linux:
	- ./build/chip8-emulator --rom roms/2-ibm-logo.ch8
	- ./build/chip8-emulator --rom roms/PONG --scale 12 --clock 700 --loadaddr 0x200
- Windows (MSYS2 MinGW64):
	- ./build/chip8-emulator.exe --rom roms/PONG
	- ./build/chip8-emulator.exe --rom roms/PONG --scale 12 --clock 700 --loadaddr 0x200

Com Make
- make run ROM=roms/PONG
- make run ROM=roms/PONG SCALE=10 CLOCK=500 LOAD=0x200

Teclas
- Sair: ESC ou fechar janela
- Mapeamento Chip-8 (PC → Chip-8):
	- 1 2 3 4  →  1 2 3 C
	- Q W E R  →  4 5 6 D
	- A S D F  →  7 8 9 E
	- Z X C V  →  A 0 B F

	
