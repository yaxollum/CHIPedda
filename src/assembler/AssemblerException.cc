#include "AssemblerException.h"
#include <sstream>

using namespace chipedda::assembler;

const char * AssemblerException::what() const noexcept
{
    return message.c_str();
}

AssemblerException::AssemblerException(const std::string& expression) 
{
    std::stringstream ss;
    ss<<"Unable to assemble expression \""<<expression<<"\"";
    message=ss.str();
}
