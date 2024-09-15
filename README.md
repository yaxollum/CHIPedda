## CHIPedda
A CHIP-8 **E**mulator, **D**isassembler, and **A**ssembler.

## Usage
```
usage: chipedda <command>

Commands:

assemble <input file> <output file>     Converts a CHIP-8 assembly file into a CHIP-8 binary.
disassemble <input file> <output file>  Converts a CHIP-8 binary into a CHIP-8 assembly file.
run <input file>                        Runs a CHIP-8 binary using the emulator.
```

## Build instructions
```bash
# clone the repository
git clone https://github.com/yaxollum/CHIPedda.git && cd CHIPedda

# build the source code
mkdir build && cd build
cmake ..
make # outputs the executable `chipedda`
```
