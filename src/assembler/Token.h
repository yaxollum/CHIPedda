#pragma once

#include <string>
#include "TokenType.h"

namespace chipedda::assembler
{
    class Token
    {
    public:
        Token(const std::string& str);
        TokenType type;
        int data;
    };
}
