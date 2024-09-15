#include <assembler/Assembler.h>
#include <cstring>
#include <disassembler/Disassembler.h>
#include <emulator/Emulator.h>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace chipedda;

int main(int argc, char **argv) {
  const char *USAGE{R"(usage: chipedda <command>

Commands:

assemble <input file> <output file>     Converts a CHIP-8 assembly file into a CHIP-8 binary.
disassemble <input file> <output file>  Converts a CHIP-8 binary into a CHIP-8 assembly file.
run <input file>                        Runs a CHIP-8 binary using the emulator.)"};
  if (argc > 1) {
    if (strcmp(argv[1], "disassemble") == 0 && argc == 4) {
      std::ifstream programFile(argv[2], std::ios::binary);
      if (!programFile.good()) {
        std::cerr << "disassembler: unable to open file \"" << argv[2]
                  << "\".\n";
        return 1;
      }
      std::vector<uint8_t> program(
          (std::istreambuf_iterator<char>(programFile)),
          std::istreambuf_iterator<char>());

      disassembler::Disassembler disassembler(program);
      std::ofstream outputFile(argv[3]);
      outputFile << disassembler.disassemble();
      return 0;
    } else if (strcmp(argv[1], "assemble") == 0 && argc == 4) {
      std::ifstream asmFile(argv[2]);
      if (!asmFile.good()) {
        std::cerr << "assembler: unable to open file \"" << argv[2] << "\".\n";
        return 1;
      }
      std::string asmCode((std::istreambuf_iterator<char>(asmFile)),
                          std::istreambuf_iterator<char>());

      assembler::Assembler assembler(asmCode);
      std::vector<uint8_t> program = assembler.assemble();
      std::ofstream outputFile(argv[3]);
      std::copy(program.begin(), program.end(),
                std::ostreambuf_iterator<char>(outputFile));
      return 0;
    } else if (strcmp(argv[1], "run") == 0 && argc == 3) {
      std::ifstream programFile(argv[2], std::ios::binary);
      if (!programFile.good()) {
        std::cerr << "emulator: unable to open file \"" << argv[2] << "\".\n";
        return 1;
      }
      std::vector<uint8_t> program(
          (std::istreambuf_iterator<char>(programFile)),
          std::istreambuf_iterator<char>());
      emulator::Emulator emulator(program);
      while (true) {
        emulator.nextInstruction();
      }
      return 0;
    }
  }
  std::cout << USAGE << "\n";
}
