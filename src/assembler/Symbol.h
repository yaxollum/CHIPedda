#pragma once

#include <map>
#include <string>

namespace chipedda::assembler
{
    enum class Symbol
    {
        NOT_AN_INSTRUCTION,
        ADD,
        AND,
        CALL,
        CLS,
        DRW,
        JP,
        LD,
        OR,
        RET,
        RND,
        SE,
        SHL,
        SHR,
        SKNP,
        SKP,
        SNE,
        SUB,
        SUBN,
        SYS,
        XOR
    };

    const std::map<std::string,Symbol> SYMBOL_MAP={
        {"ADD",Symbol::ADD},
        {"AND",Symbol::AND},
        {"CALL",Symbol::CALL},
        {"CLS",Symbol::CLS},
        {"DRW",Symbol::DRW},
        {"JP",Symbol::JP},
        {"LD",Symbol::LD},
        {"OR",Symbol::OR},
        {"RET",Symbol::RET},
        {"RND",Symbol::RND},
        {"SE",Symbol::SE},
        {"SHL",Symbol::SHL},
        {"SHR",Symbol::SHR},
        {"SKNP",Symbol::SKNP},
        {"SKP",Symbol::SKP},
        {"SNE",Symbol::SNE},
        {"SUB",Symbol::SUB},
        {"SUBN",Symbol::SUBN},
        {"SYS",Symbol::SYS},
        {"XOR",Symbol::XOR}
    };
}
