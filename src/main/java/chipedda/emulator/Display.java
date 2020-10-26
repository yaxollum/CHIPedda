package chipedda.emulator;

public class Display
{   
    private boolean[][] pixels; // two-dimensional array storing the value of
                                // each monochrome pixel (false for unset, true for set)
    private int sizeX; // horizontal size in pixels
    private int sizeY; // vertical size in pixels

    public void clear() // clears the display
    {
        for(int i=0;i<sizeY;++i)
        {
            for(int j=0;j<sizeX;++j)
            {
                pixels[i][j]=false;
            }
        }
    }
    public boolean drawSprite(byte[] memory,short address,int x,int y,int n)
    {
        boolean collision=false;
        for(int i=0;i<n;++i)
        {
            byte b=memory[address+i]; // read byte from memory
            for(int j=0;j<8;++j)
            {
                boolean pixelSet=((b>>(7-j))&1)!=0;

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

    public Display(int x,int y)
    {
        sizeX=x;
        sizeY=y;
        pixels=new boolean[x][y];
    }
}
