#include <stdint.h>

namespace chipedda::emulator
{
    class Keyboard
    {
    public:
        bool keyPressed(uint8_t keyValue);
        uint8_t waitForKeyPress();
    };
}
