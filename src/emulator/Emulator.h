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
        std::stack<uint8_t> call_stack;
        // Keyboard keyboard;
        Timer soundTimer;
        Timer delayTimer;
        void nextInstruction();
    public:
        Emulator(std::vector<uint8_t> program);
    };
}
