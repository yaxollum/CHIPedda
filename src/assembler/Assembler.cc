#include "Assembler.h"
#include "AssemblerException.h"
#include <sstream>

using namespace chipedda::assembler;

Assembler::Assembler(const std::string& asmCode) : assemblyCode(asmCode) {} 

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

std::pair<uint8_t,uint8_t> Assembler::assembleTokenSplitter(const TokenSplitter& tokenSplitter)
{
    const std::vector<Token>& tokens=tokenSplitter.getTokens();

    int nibble1=-1,nibble2=-1,nibble3=-1,nibble4=-1;
    switch(tokenSplitter.getInstruction())
    {
        case Symbol::ADD:
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
        case Symbol::AND:
            if(tokens.size()!=2)
            {
                break;
            }
            if(tokens[0].type==TokenType::V && // AND Vx, Vy - 8xy2
                    tokens[1].type==TokenType::V)
            {
                nibble1=0x8;
                nibble2=tokens[0].data;
                nibble3=tokens[1].data;
                nibble4=0x2;
            }
            break;
        case Symbol::CALL:
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
        case Symbol::CLS:
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
        case Symbol::DRW:
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
        case Symbol::JP:
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
        case Symbol::LD:
            if(tokens.size()!=2)
            {
                break;
            }
            if(tokens[0].type==TokenType::B && // LD B, Vx - Fx33
                    tokens[1].type==TokenType::V)
            {
                nibble1=0xf;
                nibble2=tokens[1].data;
                nibble3=0x3;
                nibble4=0x3;
            }
            else if(tokens[0].type==TokenType::DT && // LD DT, Vx - Fx15
                    tokens[1].type==TokenType::V)
            {
                nibble1=0xf;
                nibble2=tokens[1].data;
                nibble3=0x1;
                nibble4=0x5;
            }
            else if(tokens[0].type==TokenType::F && // LD F, Vx - Fx29
                    tokens[1].type==TokenType::V)
            {
                nibble1=0xf;
                nibble2=tokens[1].data;
                nibble3=0x2;
                nibble4=0x9;
            }
            else if(tokens[0].type==TokenType::I && // LD I, addr - Annn
                    tokens[1].type==TokenType::LITERAL)
            {
                nibble1=0xa;
                nibble2=tokens[1].data >> 8;
                nibble3=(tokens[1].data & 0xff) >> 4;
                nibble4=tokens[1].data & 0x0f;
            }
            else if(tokens[0].type==TokenType::ST && // LD ST, Vx - Fx18
                    tokens[1].type==TokenType::V)
            {
                nibble1=0xf;
                nibble2=tokens[1].data;
                nibble3=0x1;
                nibble4=0x8;
            }
            else if(tokens[0].type==TokenType::V && // LD Vx, DT - Fx07
                    tokens[1].type==TokenType::DT)
            {
                nibble1=0xf;
                nibble2=tokens[0].data;
                nibble3=0x0;
                nibble4=0x7;
            }
            else if(tokens[0].type==TokenType::V && // LD Vx, K - Fx0A
                    tokens[1].type==TokenType::K)
            {
                nibble1=0xf;
                nibble2=tokens[0].data;
                nibble3=0x0;
                nibble4=0xa;
            }
            else if(tokens[0].type==TokenType::V && // LD Vx, Vy - 8xy0
                    tokens[1].type==TokenType::V)
            {
                nibble1=0x8;
                nibble2=tokens[0].data;
                nibble3=tokens[1].data;
                nibble4=0x0;
            }
            else if(tokens[0].type==TokenType::V && // LD Vx, [I] - Fx65
                    tokens[1].type==TokenType::I_DEREF)
            {
                nibble1=0xf;
                nibble2=tokens[0].data;
                nibble3=0x6;
                nibble4=0x5;
            }
            else if(tokens[0].type==TokenType::V && // LD Vx, byte - 6xkk
                    tokens[1].type==TokenType::LITERAL)
            {
                nibble1=0x6;
                nibble2=tokens[0].data;
                nibble3=tokens[1].data >> 4;
                nibble4=tokens[1].data & 0x0f;
            }
            else if(tokens[0].type==TokenType::I_DEREF && // LD [I], Vx - Fx55
                    tokens[1].type==TokenType::V)
            {
                nibble1=0xf;
                nibble2=tokens[1].data;
                nibble3=0x5;
                nibble4=0x5;
            }
            break;
        case Symbol::OR:
            if(tokens.size()!=2)
            {
                break;
            }
            if(tokens[0].type==TokenType::V && // OR Vx, Vy - 8xy1
                    tokens[1].type==TokenType::V)
            {
                nibble1=0x8;
                nibble2=tokens[0].data;
                nibble3=tokens[1].data;
                nibble4=0x1;
            }
            break;
        case Symbol::RET:
            if(tokens.size()!=0)
            {
                break;
            }
            // RET - 00EE
            nibble1=0x0;
            nibble2=0x0;
            nibble3=0xe;
            nibble4=0xe;
            break;
        case Symbol::RND:
            if(tokens.size()!=2)
            {
                break;
            }
            if(tokens[0].type==TokenType::V && // RND Vx, byte - Cxkk
                    tokens[1].type==TokenType::LITERAL)
            {
                nibble1=0xc;
                nibble2=tokens[0].data;
                nibble3=tokens[1].data >> 4;
                nibble4=tokens[1].data & 0x0f;
            }
            break;
        case Symbol::SE:
            if(tokens.size()!=2)
            {
                break;
            }
            if(tokens[0].type==TokenType::V && // SE Vx, Vy - 5xy0
                    tokens[1].type==TokenType::V)
            {
                nibble1=0x5;
                nibble2=tokens[0].data;
                nibble3=tokens[1].data;
                nibble4=0x0;
            }
            else if(tokens[0].type==TokenType::V && // SE Vx, byte - 3xkk
                    tokens[1].type==TokenType::LITERAL)
            {
                nibble1=0x3;
                nibble2=tokens[0].data;
                nibble3=tokens[1].data >> 4;
                nibble4=tokens[1].data & 0x0f;
            }
            break;
        case Symbol::SHL:
            if(tokens.size()!=2)
            {
                break;
            }
            if(tokens[0].type==TokenType::V && // SHL Vx {, Vy} - 8xyE
                    tokens[1].type==TokenType::V)
            {
                nibble1=0x8;
                nibble2=tokens[0].data;
                nibble3=tokens[1].data;
                nibble4=0xe;
            }
            break;
        case Symbol::SHR:
            if(tokens.size()!=2)
            {
                break;
            }
            if(tokens[0].type==TokenType::V && // SHR Vx {, Vy} - 8xy6
                    tokens[1].type==TokenType::V)
            {
                nibble1=0x8;
                nibble2=tokens[0].data;
                nibble3=tokens[1].data;
                nibble4=0x6;
            }
            break;
        case Symbol::SKNP:
            if(tokens.size()!=1)
            {
                break;
            }
            if(tokens[0].type==TokenType::V) // SKNP Vx - ExA1
            {
                nibble1=0xe;
                nibble2=tokens[0].data;
                nibble3=0xa;
                nibble4=0x1;
            }
            break;
        case Symbol::SKP:
            if(tokens.size()!=1)
            {
                break;
            }
            if(tokens[0].type==TokenType::V) // SKP Vx - Ex9E
            {
                nibble1=0xe;
                nibble2=tokens[0].data;
                nibble3=0x9;
                nibble4=0xe;
            }
            break;
        case Symbol::SNE:
            if(tokens.size()!=2)
            {
                break;
            }
            if(tokens[0].type==TokenType::V && // SNE Vx, Vy - 9xy0
                    tokens[1].type==TokenType::V)
            {
                nibble1=0x9;
                nibble2=tokens[0].data;
                nibble3=tokens[1].data;
                nibble4=0x0;
            }
            else if(tokens[0].type==TokenType::V && // SNE Vx, byte - 4xkk
                    tokens[1].type==TokenType::LITERAL)
            {
                nibble1=0x4;
                nibble2=tokens[0].data;
                nibble3=tokens[1].data >> 4;
                nibble4=tokens[1].data & 0x0f;
            }
            break;
        case Symbol::SUB:
            if(tokens.size()!=2)
            {
                break;
            }
            if(tokens[0].type==TokenType::V && // SUB Vx, Vy - 8xy5
                    tokens[1].type==TokenType::V)
            {
                nibble1=0x8;
                nibble2=tokens[0].data;
                nibble3=tokens[1].data;
                nibble4=0x5;
            }
            break;
        case Symbol::SUBN:
            if(tokens.size()!=2)
            {
                break;
            }
            if(tokens[0].type==TokenType::V && // SUBN Vx, Vy - 8xy7
                    tokens[1].type==TokenType::V)
            {
                nibble1=0x8;
                nibble2=tokens[0].data;
                nibble3=tokens[1].data;
                nibble4=0x7;
            }
            break;
        case Symbol::SYS:
            if(tokens.size()!=1)
            {
                break;
            }
            if(tokens[0].type==TokenType::LITERAL) // SYS addr - 0nnn
            {
                nibble1=0x0;
                nibble2=tokens[0].data >> 8;
                nibble3=(tokens[0].data & 0xff) >> 4;
                nibble4=tokens[0].data & 0x0f;
            }
            break;
        case Symbol::XOR:
            if(tokens.size()!=2)
            {
                break;
            }
            if(tokens[0].type==TokenType::V && // XOR Vx, Vy - 8xy3
                    tokens[1].type==TokenType::V)
            {
                nibble1=0x8;
                nibble2=tokens[0].data;
                nibble3=tokens[1].data;
                nibble4=0x3;
            }
            break;
        default:
            break;
    }
    if(nibble1==-1 || nibble2==-1 || nibble3==-1 || nibble4==-1)
    {
        throw AssemblerException(tokenSplitter.getLine());
    }
    uint8_t byte1=uint8_t(nibble1) << 4 | nibble2;
    uint8_t byte2=uint8_t(nibble3) << 4 | nibble4;
    return std::make_pair(byte1,byte2);
}
