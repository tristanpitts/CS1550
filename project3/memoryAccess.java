public class memoryAccess
{
  public char operation;
  public long address;
  public int cycles;

  public memoryAccess(String s)
  {
    String[] temp = s.split(" ", 0);
    operation = temp[0].charAt(0);
    address = Long.decode(temp[1]);
    cycles = Integer.parseInt(temp[2]);
  }
}
