#pragma once

#include <string>
#include <vector>
#include "Symbol.h"
#include "Token.h"

namespace chipedda::assembler
{
    class TokenSplitter
    {
    public:
        TokenSplitter(const std::string& ln);
        bool isInstruction() const;
        Symbol getInstruction() const;
        const std::string& getLine() const;
        const std::vector<Token>& getTokens() const;
    private:
        static Symbol getInstructionFromName(const std::string& instructionName);
        static std::vector<std::string> splitLine(const std::string& ln);
        std::vector<Token> tokens;
        const std::string& line;
        Symbol instruction;
};
}
