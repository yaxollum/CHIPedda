#include "Display.h"

using namespace chipedda::emulator;

void Display::clear() // clears the display
{
    for(int i=0;i<sizeY;++i)
    {
        for(int j=0;j<sizeX;++j)
        {
            pixels[i][j]=false;
        }
    }
}
bool Display::drawSprite(const std::vector<uint8_t>& memory,uint16_t address,int x,int y,int n)
{
    bool collision=false;
    for(int i=0;i<n;++i)
    {
        uint8_t b=memory[address+i]; // read byte from memory
        for(int j=0;j<8;++j)
        {
            bool pixelSet=((b>>(7-j))&1)!=0;

            // wrap around to the opposite side of the screen if pixel is drawn off the screen
            int displayCoordX=(x+j)%sizeX; 
            int displayCoordY=(y+i)%sizeY;

            if(pixels[displayCoordY][displayCoordX] && pixelSet)
            {
                collision=true; // an existing pixel was erased (true XOR true == true)
            }
            pixels[displayCoordY][displayCoordX]^=pixelSet; // XOR the new pixel with existing pixel
        }
    }
    return collision;
}

Display::Display(int x,int y) : sizeX(x),sizeY(y),pixels(y,std::vector<char>(x)) {}
