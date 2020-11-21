#include <stdint.h>

namespace chipedda::emulator
{
    // A timer that counts down at 60Hz
    class Timer
    {
    public:
        void setValue(uint8_t ticks);
    private:
        int64_t lastSetTime; // time when the timer was last set
    };
}
