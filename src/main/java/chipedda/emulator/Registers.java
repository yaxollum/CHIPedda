package chipedda.emulator;

public class Registers
{
    public byte[] generalPurpose = new byte[16]; // V0, V1, V2, ... VF
    public short registerI; 
    public byte soundTimer, delayTimer;
    public short programCounter;
}
