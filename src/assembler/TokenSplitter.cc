#include "TokenSplitter.h"
#include "AssemblerException.h"
#include <sstream>

using namespace chipedda::assembler;

TokenSplitter::TokenSplitter(const std::string& ln) : line(ln)
{
    std::istringstream lineStream(line);
    std::string instructionName;
    lineStream >> instructionName;

    instruction=getInstructionFromName(instructionName);
    if(instruction!=Symbol::NOT_AN_INSTRUCTION)
    {
        tokens=splitTokens(lineStream);
    }
}

Symbol TokenSplitter::getInstructionFromName(const std::string& instructionName)
{
    bool expressionIsInstruction=false;
    if(instructionName.length()==0 || instructionName[0]==';')
    {
        return Symbol::NOT_AN_INSTRUCTION;
    }
    else
    {
        auto it=SYMBOL_MAP.find(instructionName);
        if(it!=SYMBOL_MAP.end())
        {
            return it->second;
        }
        else
        {
            throw AssemblerException(instructionName);
        }
    }
}

std::vector<Token> TokenSplitter::splitTokens(std::istream& istr)
{
    std::vector<Token> split;
    std::string next;
    while(std::getline(istr,next,','))
    {
        split.push_back(Token(next));
    }
    return split;
}

bool TokenSplitter::isInstruction()
{
    return instruction!=Symbol::NOT_AN_INSTRUCTION;
}
