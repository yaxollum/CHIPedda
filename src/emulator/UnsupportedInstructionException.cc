#include "UnsupportedInstructionException.h"
#include <sstream>

using namespace chipedda::emulator;

const char * UnsupportedInstructionException::what() const noexcept
{
    return message.c_str();
}

UnsupportedInstructionException::UnsupportedInstructionException(uint16_t instr) : instruction(instr)
{
    std::stringstream ss;
    ss<<"Instruction "<<instruction<<" not supported!";
    message=ss.str();
}
