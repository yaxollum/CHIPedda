/* information gathered from "Cowgod's Chip-8 Technical Reference"
 * by Thomas P. Greene (http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
 */

#include "Disassembler.h"
#include "instruction/UnsupportedInstructionException.h"
#include <sstream>
#include <iomanip>

using namespace chipedda::disassembler;
using namespace chipedda::instruction;

Disassembler::Disassembler(const std::vector<uint8_t>& prog) : program(prog) {}

std::string Disassembler::intToHexString(int num,int padding)
{
    std::stringstream ss;
    ss<<"0x"<<std::hex<<std::setfill('0')<<std::setw(padding)<<num;
    return ss.str();
}

std::string Disassembler::getRegisterName(uint8_t x)
{
    std::stringstream ss;
    ss<<"V"<<std::uppercase<<std::hex<<(int)x;
    return ss.str();
}

std::string Disassembler::disassemble()
{
    std::string assemblyCode="";
    for(int i=0;i<program.size()-1;i+=2) // loop up to the second-last byte of program
    {
        assemblyCode+=disassembleInstruction(program[i],program[i+1]);
        assemblyCode+='\n';
    }
    return assemblyCode;
}

std::string Disassembler::disassembleInstruction(uint8_t byte1,uint8_t byte2) // diassemble next instruction
{
    uint16_t full=(byte1<<8)+byte2; // full instruction

    // four nibbles of instruction
    int nibble1=byte1 >> 4;
    int nibble2=byte1 & 0x0f;
    int nibble3=byte2 >> 4;
    int nibble4=byte2 & 0x0f;

    uint16_t addr=full & 0xfff; // last three nibbles of instruction

    std::string byte1_str=intToHexString(byte1,2);
    std::string byte2_str=intToHexString(byte2,2);

    std::string full_str=intToHexString(full,4);

    std::string nibble1_str=intToHexString(nibble1,1);
    std::string nibble2_str=intToHexString(nibble2,1);
    std::string nibble3_str=intToHexString(nibble3,1);
    std::string nibble4_str=intToHexString(nibble4,1);

    std::string nibble2_reg=getRegisterName(nibble2);
    std::string nibble3_reg=getRegisterName(nibble3);

    std::string addr_str=intToHexString(addr,3);

    std::stringstream assemblyStream;

    switch(nibble1)
    {
        case(0x0):
            if(byte1==0x00 && byte2==0xe0) // 00E0 - CLS
            {
                assemblyStream<<"CLS";
            }
            else if(byte1==0x00 && byte2==0xee) // 00EE - RET
            {
                assemblyStream<<"RET";
            }
            else // 0nnn - SYS addr
            {
                assemblyStream<<"SYS "<<addr_str;
            }
            break;
        case(0x1): // 1nnn - JP addr
            assemblyStream<<"JP "<<addr_str;
            break;
        case(0x2): // 2nnn - CALL addr
            assemblyStream<<"CALL "<<addr_str;
            break;
        case(0x3): // 3xkk - SE Vx, byte
            assemblyStream<<"SE "<<nibble2_reg<<", "<<byte2_str;
            break;
        case(0x4): // 4xkk - SNE Vx, byte
            assemblyStream<<"SNE "<<nibble2_reg<<", "<<byte2_str;
            break;
        case(0x5): // 5xy0 - SE Vx, Vy
            if(nibble4==0x0)
            {
                assemblyStream<<"SE "<<nibble2_reg<<", "<<nibble3_reg;
                break;
            }
        case(0x6): // 6xkk - LD Vx, byte
            assemblyStream<<"LD "<<nibble2_reg<<", "<<byte2_str;
            break;
        case(0x7): // 7xkk - ADD Vx, byte
            assemblyStream<<"ADD "<<nibble2_reg<<", "<<byte2_str;
            break;
        case(0x8):
            switch(nibble4)
            {
                case(0x0): // 8xy0 - LD Vx, Vy
                    assemblyStream<<"LD "<<nibble2_reg<<", "<<nibble3_reg;
                    break;
                case(0x1): // 8xy1 - OR Vx, Vy
                    assemblyStream<<"OR "<<nibble2_reg<<", "<<nibble3_reg;
                    break;
                case(0x2): // 8xy2 - AND Vx, Vy
                    assemblyStream<<"AND "<<nibble2_reg<<", "<<nibble3_reg;
                    break;
                case(0x3): // 8xy3 - XOR Vx, Vy
                    assemblyStream<<"XOR "<<nibble2_reg<<", "<<nibble3_reg;
                    break;
                case(0x4): // 8xy4 - ADD Vx, Vy
                    assemblyStream<<"ADD "<<nibble2_reg<<", "<<nibble3_reg;
                    break;
                case(0x5): // 8xy5 - SUB Vx, Vy
                    assemblyStream<<"SUB "<<nibble2_reg<<", "<<nibble3_reg;
                    break;
                case(0x6): // 8xy6 - SHR Vx
                    assemblyStream<<"SHR "<<nibble2_reg;
                    break;
                case(0x7): // 8xy7 - SUBN Vx, Vy
                    assemblyStream<<"SUBN "<<nibble2_reg<<", "<<nibble3_reg;
                    break;
                case(0xe): // 8xyE - SHL Vx
                    assemblyStream<<"SHL "<<nibble2_reg;
                    break;
                default:
                    throw UnsupportedInstructionException(full);
            }
            break;
        case(0x9): // 9xy0 - SNE Vx, Vy
            if(nibble4==0x0)
            {
                assemblyStream<<"SNE "<<nibble2_reg<<", "<<nibble3_reg;
                break;
            }
        case(0xa): // Annn - LD I, addr
            assemblyStream<<"LD I, "<<addr_str;
            break;
        case(0xb): // Bnnn - JP V0, addr 
            assemblyStream<<"JP V0, "<<addr_str;
            break;
        case(0xc): // Cxkk - RND Vx, byte
        {
            assemblyStream<<"RND "<<nibble2_reg<<", "<<byte2_str;
            break;
        }
        case(0xd): // Dxyn - DRW Vx, Vy, nibble
            assemblyStream<<"DRW "<<nibble2_reg<<", "<<nibble3_reg<<", "<<nibble4_str;
            break;
        case(0xe):
            switch(byte2)
            {
                case(0x9e): // Ex9E - SKP Vx
                    assemblyStream<<"SKP "<<nibble2_reg;
                    break;
                case(0xa1): // ExA1 - SKNP Vx
                    assemblyStream<<"SKNP "<<nibble2_reg;
                    break;
                default:
                    throw UnsupportedInstructionException(full);
            }
            break;
        case(0xf):
            switch(byte2)
            {
                case(0x07): // Fx07 - LD Vx, DT
                    assemblyStream<<"LD "<<nibble2_reg<<", DT";
                    break;
                case(0x0A): // Fx0A - LD Vx, K
                    assemblyStream<<"LD "<<nibble2_reg<<", K";
                    break;
                case(0x15): // Fx15 - LD DT, Vx
                    assemblyStream<<"LD DT, "<<nibble2_reg;
                    break;
                case(0x18): // Fx18 - LD ST, Vx
                    assemblyStream<<"LD ST, "<<nibble2_reg;
                    break;
                case(0x1E): // Fx1E - ADD I, Vx
                    assemblyStream<<"ADD I, "<<nibble2_reg;
                    break;
                case(0x29): // Fx29 - LD F, Vx
                    assemblyStream<<"LD F, "<<nibble2_reg;
                    break;
                case(0x33): // Fx33 - LD B, Vx
                    assemblyStream<<"LD B, "<<nibble2_reg;
                    break;
                case(0x55): // Fx55 - LD [I], Vx
                    assemblyStream<<"LD [I], "<<nibble2_reg;
                    break;
                case(0x65): // Fx65 - LD Vx, [I]
                    assemblyStream<<"LD "<<nibble2_reg<<", [I]";
                    break;
                default:
                    throw UnsupportedInstructionException(full);
            }
            break;
        default:
            throw UnsupportedInstructionException(full);
    }
    return assemblyStream.str();
}

