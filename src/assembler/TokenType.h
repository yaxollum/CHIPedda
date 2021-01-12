#pragma once

#include <map>
#include <string>

namespace chipedda::assembler
{
    enum class TokenType
    {
        I,
        V, // Vx
        LITERAL, // byte, addr, or nibble
        B,
        DT,
        F,
        ST,
        K,
        I_DEREF, // [I]
    };
    const std::map<std::string,TokenType> TOKEN_TYPE_MAP={
        {"I",TokenType::I},
        {"B",TokenType::B},
        {"DT",TokenType::DT},
        {"F",TokenType::F},
        {"ST",TokenType::ST},
        {"K",TokenType::K},
        {"[I]",TokenType::I_DEREF},
    };
}
