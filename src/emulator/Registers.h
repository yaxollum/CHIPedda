#include <stdint.h>

namespace chipedda::emulator
{
    class Registers
    {
    public:
        uint8_t generalPurpose[16]; // V0, V1, V2, ... VF
        uint16_t registerI; 
        uint8_t soundTimer, delayTimer;
        uint16_t programCounter;
    };
}
