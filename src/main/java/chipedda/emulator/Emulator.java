package chipedda.emulator;

public class Emulator
{
    private byte[] memory=new byte[0xfff];
    private Display display;
    private Registers registers;
    private CallStack stack;
}
