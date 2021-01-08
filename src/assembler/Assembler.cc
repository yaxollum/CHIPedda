#include "Assembler.h"
#include "AssemblerException.h"
#include <sstream>

using namespace chipedda::assembler;

std::vector<uint8_t> Assembler::assemble()
{
    std::vector<uint8_t> binaryProgram;

    std::istringstream assemblyCodeStream(assemblyCode);

    std::string line;
    while(std::getline(assemblyCodeStream,line))
    {
        TokenSplitter tokenSplitter(line);
        if(tokenSplitter.isInstruction())
        {
            std::pair<uint8_t,uint8_t> assembledInstruction=assembleTokenSplitter(tokenSplitter);
            binaryProgram.push_back(assembledInstruction.first);
            binaryProgram.push_back(assembledInstruction.second);
        }
    }

    return binaryProgram;
}

std::pair<uint8_t,uint8_t> assembleTokenSplitter(const TokenSplitter& tokenSplitter)
{
    std::vector<Token> tokens=tokenSplitter.getTokens();

    int nibble1=-1,nibble2=-1,nibble3=-1,nibble4=-1;
    switch(tokenSplitter.getInstruction())
    {
        case(Symbol::ADD):
            if(tokens.size()!=2)
            {
                break;
            }
            if(tokens[0].type==TokenType::I &&  // ADD I, Vx - Fx1E
                    tokens[1].type==TokenType::V)
            {
                nibble1=0xf;
                nibble2=tokens[1].data;
                nibble3=0x1;
                nibble4=0xe;
            }
            else if(tokens[0].type==TokenType::V && // ADD Vx, Vy - 8xy4
                    tokens[1].type==TokenType::V)
            {
                nibble1=0x8;
                nibble2=tokens[0].data;
                nibble3=tokens[1].data;
                nibble4=0x4;
            }
            else if(tokens[0].type==TokenType::V && // ADD Vx, byte - 7xkk
                    tokens[1].type==TokenType::LITERAL)
            {
                nibble1=0x7;
                nibble2=tokens[0].data;
                nibble3=tokens[1].data >> 4;
                nibble4=tokens[1].data & 0x0f;
            }
            break;
        case(Symbol::CALL):
            if(tokens.size()!=1)
            {
                break;
            }
            if(tokens[0].type==TokenType::LITERAL) // CALL addr - 2nnn
            {
                nibble1=0x2;
                nibble2=tokens[0].data >> 8;
                nibble3=(tokens[0].data & 0xff) >> 4;
                nibble4=tokens[0].data & 0x0f;
            }
            break;
        case(Symbol::CLS):
            if(tokens.size()!=0)
            {
                break;
            }
            // CLS - 00E0
            nibble1=0x0;
            nibble2=0x0;
            nibble3=0xe;
            nibble4=0x0;
            break;
        case(Symbol::DRW):
            if(tokens.size()!=3)
            {
                break;
            }
            if(tokens[0].type==TokenType::V && // DRW Vx, Vy, nibble - Dxyn
                    tokens[1].type==TokenType::V &&
                    tokens[2].type==TokenType::LITERAL)
            {
                nibble1=0xd;
                nibble2=tokens[0].data;
                nibble3=tokens[1].data;
                nibble4=tokens[2].data;
            }
            break;
        case(Symbol::JP):
            if(tokens.size()==2)
            {
                if(tokens[0].type==TokenType::V && // JP V0, addr - Bnnn
                        tokens[0].data==0 &&
                        tokens[1].type==TokenType::LITERAL)
                {
                    nibble1=0xb;
                    nibble2=tokens[1].data >> 8;
                    nibble3=(tokens[1].data & 0xff) >> 4;
                    nibble4=tokens[1].data & 0x0f;
                }

            }
            else if(tokens.size()==1)
            {
                if(tokens[0].type==TokenType::LITERAL) // JP addr - 1nnn
                {
                    nibble1=0x1;
                    nibble2=tokens[0].data >> 8;
                    nibble3=(tokens[0].data & 0xff) >> 4;
                    nibble4=tokens[0].data & 0x0f;
                }
            }
            break;
        case(Symbol::LD):
            if(tokens.size()!=2)
            {
                break;
            }

            break;
        default:
            break;
    }
    if(nibble1==-1 || nibble2==-1 || nibble3==-1 || nibble4==-1)
    {
        throw AssemblerException(tokenSplitter.getLine());
    }
}

