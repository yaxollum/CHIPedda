/* information gathered from "Cowgod's Chip-8 Technical Reference"
 * by Thomas P. Greene (http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
 */

#include "Emulator.h"
#include "sprites/Sprites.h"
#include "instruction/UnsupportedInstructionException.h"
#include <stdio.h>
#include <chrono>

using namespace chipedda::instruction;
using namespace chipedda::emulator;
using namespace chipedda::emulator::sprites;

Emulator::Emulator(const std::vector<uint8_t>& program) : 
    memory(0x1000),
    display(64,32),
    mtEngine(static_cast<uint32_t>(std::chrono::high_resolution_clock::now().time_since_epoch().count()))
{
    std::copy(program.begin(),program.end(),memory.begin()+200); // copy program to memory
    registers.programCounter=0x200; // programs start at address 0x200

    std::copy(SPRITES.begin(),SPRITES.end(),memory.begin()); // copy sprites to memory
}

void Emulator::nextInstruction() // execute next instruction
{
    uint16_t &pc=registers.programCounter;
    uint16_t &I=registers.registerI;
    uint8_t *V=registers.generalPurpose;

    uint8_t byte1=memory[pc]; // first byte of instruction
    uint8_t byte2=memory[pc+1]; // second byte of instruction

    uint16_t full=(byte1<<8)+byte2; // full instruction

    // four nibbles of instruction
    int nibble1=byte1 >> 4;
    int nibble2=byte1 & 0x0f;
    int nibble3=byte2 >> 4;
    int nibble4=byte2 & 0x0f;

    uint16_t addr=full & 0xfff; // last three nibbles of instruction


    switch(nibble1)
    {
        case 0x0:
            if(byte1==0x00 && byte2==0xe0) // 00E0 - CLS
            {
                display.clear();
            }
            else if(byte1==0x00 && byte2==0xee) // 00EE - RET
            {
                pc=call_stack.top();
                call_stack.pop();
            }
            else // 0nnn - SYS addr
            {
                // ignore this instruction
            }
            pc+=2;
            break;
        case 0x1: // 1nnn - JP addr
            pc=addr;
            break;
        case 0x2: // 2nnn - CALL addr
            call_stack.push(pc);
            pc=addr;
            break;
        case 0x3: // 3xkk - SE Vx, byte
            if(V[nibble2]==byte2)
            {
                pc+=4; // skip next instruction if equal
            }
            else
            {
                pc+=2;
            }
            break;
        case 0x4: // 4xkk - SNE Vx, byte
            if(V[nibble2]!=byte2)
            {
                pc+=4; // skip next instruction if not equal
            }
            else
            {
                pc+=2;
            }
            break;
        case 0x5: // 5xy0 - SE Vx, Vy
            if(nibble4==0x0)
            {
                if(V[nibble2]==V[nibble3])
                {
                    pc+=4; // skip next instruction if equal
                }
                else
                {
                    pc+=2;
                }
            }
            else
            {
                throw UnsupportedInstructionException(full);
            }
            break;
        case 0x6: // 6xkk - LD Vx, byte
            V[nibble2]=byte2;
            pc+=2;
            break;
        case 0x7: // 7xkk - ADD Vx, byte
            V[nibble2]+=byte2;
            pc+=2;
            break;
        case 0x8:
            switch(nibble4)
            {
                case 0x0: // 8xy0 - LD Vx, Vy
                    V[nibble2]=V[nibble3];
                    break;
                case 0x1: // 8xy1 - OR Vx, Vy
                    V[nibble2] |= V[nibble3];
                    break;
                case 0x2: // 8xy2 - AND Vx, Vy
                    V[nibble2] &= V[nibble3];
                    break;
                case 0x3: // 8xy3 - XOR Vx, Vy
                    V[nibble2] ^= V[nibble3];
                    break;
                case 0x4: // 8xy4 - ADD Vx, Vy
                    V[0xf]=V[nibble2] > UINT8_MAX-V[nibble3] ? 1 : 0;
                    V[nibble2]+=V[nibble3];
                    break;
                case 0x5: // 8xy5 - SUB Vx, Vy
                    V[0xf]=V[nibble2]>V[nibble3] ? 1 : 0;
                    V[nibble2]-=V[nibble3];
                    break;
                case 0x6: // 8xy6 - SHR Vx {, Vy}
                    V[0xf]=V[nibble2] & 1;
                    V[nibble2]>>=1;
                    break;
                case 0x7: // 8xy7 - SUBN Vx, Vy
                    V[0xf]=V[nibble3]>V[nibble2] ? 1 : 0;
                    V[nibble2]=V[nibble3]-V[nibble2];
                    break;
                case 0xe: // 8xyE - SHL Vx {, Vy}
                    V[0xf]=V[nibble2] & (1<<7);
                    V[nibble2]<<=1;
                    break;
                default:
                    throw UnsupportedInstructionException(full);
            }
            pc+=2;
            break;
        case 0x9: // 9xy0 - SNE Vx, Vy
            if(nibble4==0x0)
            {
                if(V[nibble2]!=V[nibble3])
                {
                    pc+=4; // skip next instruction if not equal
                }
                else
                {
                    pc+=2;
                }
            }
            else
            {
                throw UnsupportedInstructionException(full);
            }
            break;
        case 0xa: // Annn - LD I, addr
            I=addr;
            pc+=2;
            break;
        case 0xb: // Bnnn - JP V0, addr 
            pc=addr+V[0];
            break;
        case 0xc: // Cxkk - RND Vx, byte
        {
            uint8_t randomByte=static_cast<uint8_t>(0xff&mtEngine());
            V[nibble2]=randomByte&byte2;
            pc+=2;
            break;
        }
        case 0xd: // Dxyn - DRW Vx, Vy, nibble
            V[0xf]=(display.drawSprite(memory,I,V[nibble2],V[nibble3],nibble4))? 1 : 0;
            pc+=2;
            break;
        case 0xe:
            switch(byte2)
            {
                case 0x9e: // Ex9E - SKP Vx
                    if(keyboard.keyPressed(V[nibble2]))
                    {
                        pc+=4;
                    }
                    else
                    {
                        pc+=2;
                    }
                    break;
                case 0xa1: // ExA1 - SKNP Vx
                    if(!keyboard.keyPressed(V[nibble2]))
                    {
                        pc+=4;
                    }
                    else
                    {
                        pc+=2;
                    }
                    break;
                default:
                    throw UnsupportedInstructionException(full);
            }
            break;
        case 0xf:
            switch(byte2)
            {
                case 0x07: // Fx07 - LD Vx, DT
                    V[nibble2]=delayTimer.getValue();
                    break;
                case 0x0A: // Fx0A - LD Vx, K
                    V[nibble2]=keyboard.waitForKeyPress();
                    break;
                case 0x15: // Fx15 - LD DT, Vx
                    delayTimer.setValue(V[nibble2]);
                    break;
                case 0x18: // Fx18 - LD ST, Vx
                    soundTimer.setValue(V[nibble2]);
                    break;
                case 0x1E: // Fx1E - ADD I, Vx
                    I+=V[nibble2];
                    break;
                case 0x29: // Fx29 - LD F, Vx
                    I=getSpriteLocation(V[nibble2]);
                    break;
                case 0x33: // Fx33 - LD B, Vx
                    memory[I]=V[nibble2]/uint8_t(100);
                    memory[I+1]=V[nibble2]/uint8_t(10)%uint8_t(10);
                    memory[I+2]=V[nibble2]%uint8_t(10);
                    break;
                case 0x55: // Fx55 - LD [I], Vx
                    std::copy(V,V+0x10,memory.begin()+I);
                    break;
                case 0x65: // Fx65 - LD Vx, [I]
                    std::copy(memory.begin()+I,memory.begin()+I+0x10,V);
                    break;
                default:
                    throw UnsupportedInstructionException(full);
            }
            pc+=2;
            break;
        default:
            throw UnsupportedInstructionException(full);
    }
}

