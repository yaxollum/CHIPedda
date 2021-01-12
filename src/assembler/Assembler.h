#pragma once

#include "TokenSplitter.h"
#include <vector>
#include <string>
#include <stdint.h>

namespace chipedda::assembler
{
    class Assembler
    {
    public:
        Assembler(const std::string& asmCode);
        std::vector<uint8_t> assemble();
    private:
        const std::string& assemblyCode;
        std::pair<uint8_t,uint8_t> assembleTokenSplitter(const TokenSplitter& tokenSplitter);
    };
}
