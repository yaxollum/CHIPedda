/* information gathered from "Cowgod's Chip-8 Technical Reference"
 * by Thomas P. Greene (http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
 */

#include "Emulator.h"

using namespace chipedda::emulator;

Emulator::Emulator(std::vector<uint8_t> program) : memory(0xfff),display(64,32)
{
    std::copy(program.begin(),program.end(),memory.begin()+200); // copy program to memory
    registers.programCounter=0x200; // programs start at address 0x200
}

void Emulator::nextInstruction() // execute next instruction
{
    uint16_t &pc=registers.programCounter;
    uint8_t *V=registers.generalPurpose;

    uint8_t byte1=memory[pc]; // first byte of instruction
    uint8_t byte2=memory[pc+1]; // second byte of instruction

    uint16_t full=(byte1<<8)+byte2; // full instruction

    // four nibbles of instruction
    int nibble1=byte1 & 0xf0;
    int nibble2=byte1 & 0x0f;
    int nibble3=byte2 & 0xf0;
    int nibble4=byte2 & 0x0f;

    uint16_t last_three_nibbles=full & 0xfff; // last three nibbles of instruction


    if(byte1==0x00 && byte2==0xe0) // 00E0 - CLS
    {
        display.clear();
    }
    else if(byte1==0x00 && byte2==0xee) // 00EE - RET
    {
        pc=call_stack.top();
        call_stack.pop();
    }
    else if(nibble1==0x0) // 0nnn - SYS addr
    {
        // ignore this instruction
    }
    else if(nibble1==0x1) // 1nnn - JP addr
    {
        pc=last_three_nibbles;
    }
    else if(nibble1==0x2) // 2nnn - CALL addr
    {
        call_stack.push(pc);
        pc=last_three_nibbles;
    }
    else if(nibble1==0x3) // 3xkk - SE Vx, byte
    {
        if(V[nibble2]==byte2)
        {
            ++pc; // skip next instruction if equal
        }
    }
    else if(nibble1==0x4) // 4xkk - SNE Vx, byte
    {
        if(V[nibble2]!=byte2)
        {
            ++pc; // skip next instruction if not equal
        }
    }
    else if(nibble1==0x5 && nibble4==0x0) // 5xy0 - SE Vx, Vy
    {
        if(V[nibble2]==V[nibble3])
        {
            ++pc; // skip next instruction if equal
        }
    }
    else if(nibble1==0x6) // 6xkk - LD Vx, byte
    {
        V[nibble2]=byte2;
    }
    else if(nibble1==0x7) // 7xkk - ADD Vx, byte
    {
        V[nibble2]+=byte2;
    }
    else if(nibble1==0x8 && nibble4==0x0) // 8xy0 - LD Vx, Vy
    {
        V[nibble2]=V[nibble3];
    }
    else if(nibble1==0x8 && nibble4==0x1) // 8xy1 - OR Vx, Vy
    {
        V[nibble2] |= V[nibble3];
    }
    else if(nibble1==0x8 && nibble4==0x2) // 8xy2 - AND Vx, Vy
    {
        V[nibble2] &= V[nibble3];
    }
    else if(nibble1==0x8 && nibble4==0x3) // 8xy3 - XOR Vx, Vy
    {
        V[nibble2] ^= V[nibble3];
    }
}
