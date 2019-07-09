public class frame
{
  int frame_number;
  int counter;
  long virtual_address;
  boolean present;
  boolean dirty;

  public frame(int f, long va, boolean p, boolean d)
  {
    frame_number = f;
    virtual_address = va;
    present = p;
    dirty = d;
    counter=0;
  }
}
