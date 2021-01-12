#include <iostream>
#include <iomanip>
#include <fstream>
#include <disassembler/Disassembler.h>
#include <assembler/Assembler.h>

using namespace chipedda::disassembler;
using namespace chipedda::assembler;

int main() 
{
    /*
    std::ifstream programFile("program",std::ios::binary);

    std::vector<uint8_t> program((std::istreambuf_iterator<char>(programFile)),std::istreambuf_iterator<char>());

    Disassembler disassembler(program);
    std::cout<<disassembler.disassemble();
    */
    std::ifstream asmFile("asm");

    std::string asmCode((std::istreambuf_iterator<char>(asmFile)),std::istreambuf_iterator<char>());

    Assembler assembler(asmCode);
    std::vector<uint8_t> program=assembler.assemble();
    std::copy(program.begin(),program.end(),std::ostreambuf_iterator<char>(std::cout));
}
