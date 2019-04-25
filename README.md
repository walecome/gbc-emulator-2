# Gameboy Color Emulator

This is a Gameboy Color emulator written in modern C++.

Building the project:

```
git clone https://github.com/walecome/gbc-emulator.git
cd gcb-emulator
mkdir build && cd build
cmake ..
make
./emulator /path/to/rom_file
```

## Mostly done:

- Processor implementation
- Memory implementation
- OPcode implementation
- ROM loading

## TODO:

- CPU interrupts
- Instruction timings
- Cartridge implementation
- Sound implementation
- Input implementation
- GUI frontend
- Double check of OPcode flag logic
- Blargg's tests
