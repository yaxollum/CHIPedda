#include "Display.h"
#include "Registers.h"
#include "Timer.h"
#include <stack>

namespace chipedda::emulator
{
    class Emulator
    {
    private:
        std::vector<uint8_t> memory;
        Display display;
        Registers registers;
        std::stack<uint8_t> stack;
        // Keyboard keyboard;
        Timer soundTimer;
        Timer delayTimer;
    };
}
