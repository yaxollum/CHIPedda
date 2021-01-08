#include "AssemblerException.h"
#include <sstream>

using namespace chipedda::assembler;

const char * AssemblerException::what() const noexcept
{
    return message.c_str();
}

AssemblerException::AssemblerException(const std::string& line) 
{
    std::stringstream ss;
    ss<<"Unable to assemble line \""<<line<<"\"";
    message=ss.str();
}
