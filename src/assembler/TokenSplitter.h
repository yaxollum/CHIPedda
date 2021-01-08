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
        TokenSplitter(const std::string& line);
        bool isInstruction() const;
        Symbol getInstruction() const;
        const std::string& getLine() const;
        const std::vector<Token>& getTokens() const;
    private:
        Symbol getInstructionFromName(const std::string& instructionName);
        std::vector<Token> splitTokens(std::istream& istr);
        std::vector<Token> tokens;
        const std::string& line;
        Symbol instruction;
};
}
