import java.io.*;
import java.util.*;
//import frame;

public class vmsim
{
  public static ArrayList<frame> physicalMemory;
  public static ArrayList<memoryAccess> accesses;
  public static HashMap<Long,Integer> pageTable;
  public static int faults;
  public static int fifo_tracker;
  public static int frames;
  public static int writes;

  public static void main(String[] args) throws Exception
  {
    if(args.length != 5 && args.length != 7)
    {
      System.out.println("USAGE: java vmsim -n <numframes> -a <fifo> [-r refresh] <tracefile>");
    }

    frames = Integer.parseInt(args[1]);
    String algorithm=args[3];

    BufferedReader in;

    if(args.length == 5)
    {
      in = new BufferedReader(new FileReader(args[4]));
    }
    else
    {
      in = new BufferedReader(new FileReader(args[6]));
    }

    String temp;
    accesses = new ArrayList<memoryAccess>();
    physicalMemory = new ArrayList<frame>();
    pageTable = new HashMap<Long, Integer>();
    faults = 0;
    writes = 0;
    int victim = -1;
    fifo_tracker = -1;

    while((temp = in.readLine()) != null)
    {
      accesses.add(new memoryAccess(temp));
    }

    boolean found;

    for(memoryAccess ma : accesses)
    {
      found = false;
      for(frame f : physicalMemory)
      {
        if(((f.virtual_address>>12) & 0xfffff) == ((ma.address>>12) & 0xfffff))
        {
          found = true;
          break;
        }
      }

      if(!found)
      {
        faults++;
        if(physicalMemory.size() < frames)
          physicalMemory.add(new frame(physicalMemory.size(), ma.address, false, 's'==ma.operation));
        else
        {
          switch (algorithm) {
            case "fifo":
              victim = fifo();
              break;
            case "opt":
              victim = opt();
              break;
            case "aging":
              victim = aging();
              break;
          }


          //System.out.println("EVICT: " + physicalMemory.get(victim).virtual_address);

          if(physicalMemory.get(victim).dirty)
          {
            writes++;
          }
          physicalMemory.set(victim, new frame(victim, ma.address, false, 's'==ma.operation));
          //System.out.println("Page op: " + ma.operation + " correct: " + physicalMemory.get(victim).dirty);
        }
      }
    }

    System.out.println("Algorithm: " + algorithm.toUpperCase());
    System.out.println("Number of frames: " + frames);
    System.out.println("Total memory accesses: " + accesses.size());
    System.out.println("Total page faults: " + faults);
    System.out.println("Total writes to disk: " + writes);
  }

  public static int fifo()
  {
    fifo_tracker++;
    fifo_tracker%=frames;
    return fifo_tracker;
  }

  public static int opt()
  {
    return 0;
  }

  public static int aging()
  {
    return 0;
  }

}
