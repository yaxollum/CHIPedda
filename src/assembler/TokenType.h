#pragma once

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
        I_DEREF, // [I]
    };
}
