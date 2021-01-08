#include "UnsupportedInstructionException.h"
#include <sstream>
#include <iomanip>

using namespace chipedda::instruction;

const char * UnsupportedInstructionException::what() const noexcept
{
    return message.c_str();
}

UnsupportedInstructionException::UnsupportedInstructionException(uint16_t instr) 
{
    std::stringstream ss;
    ss<<"Instruction 0x"<<std::hex<<std::setfill('0')<<std::setw(4)<<instr<<" not supported!";
    message=ss.str();
}
