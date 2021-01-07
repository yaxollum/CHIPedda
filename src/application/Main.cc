#include <iostream>
#include <iomanip>
#include <fstream>
#include <disassembler/Disassembler.h>

using namespace chipedda::disassembler;

int main() 
{
    std::ifstream programFile("program",std::ios::binary);

    std::vector<uint8_t> program((std::istreambuf_iterator<char>(programFile)),std::istreambuf_iterator<char>());

    Disassembler disassembler(program);
    std::cout<<disassembler.disassemble();
}
