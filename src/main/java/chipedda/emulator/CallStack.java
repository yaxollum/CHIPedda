package chipedda.emulator;

public class CallStack
{
    private short[] addresses;
    private int stackPointer;
    
    public CallStack(int stackSize)
    {
        addresses=new short[stackSize];
        stackPointer=-1;
    }
    public void pushAddress(short address)
    {
        ++stackPointer;
        addresses[stackPointer]=address;
    }
    public short popAddress()
    {
        return addresses[stackPointer--];
    }
}
