#include <stdint.h>
#include <vector>

namespace chipedda::emulator
{
    class Display
    {   
    public:
        void clear(); // clears the display
        bool drawSprite(const std::vector<uint8_t>& memory,uint16_t address,int x,int y,int n);
        Display(int x,int y);
    private:
        std::vector<std::vector<char>> pixels; // two-dimensional array storing the value of
                                               // each monochrome pixel (0 for unset, 1 for set)
        int sizeX; // horizontal size in pixels
        int sizeY; // vertical size in pixels
    };
}
