import java.io.*;
import java.util.*;
//import frame;

public class vmsim
{
  public static ArrayList<frame> physicalMemory;
  public static ArrayList<memoryAccess> accesses;
  public static HashMap<Long, Integer> pageTable;
  public static HashMap<Long, ArrayList<Integer>> optAccesses;
  public static int faults;
  public static int fifo_tracker;
  public static int frames;
  public static int writes;
  public static int count;
  public static int ticks;
  public static int refresh;

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
      refresh = Integer.parseInt(args[5]);
    }

    String temp;
    count = 0;
    ticks=0;
    accesses = new ArrayList<memoryAccess>();
    physicalMemory = new ArrayList<frame>();
    pageTable = new HashMap<Long, Integer>();
    optAccesses = new HashMap<Long, ArrayList<Integer>>();
    faults = 0;
    writes = 0;
    int victim = -1;
    fifo_tracker = -1;

    while((temp = in.readLine()) != null)
    {
      accesses.add(new memoryAccess(temp));
    }

    if(algorithm.equals("opt"))
    {
      for(memoryAccess a : accesses)
      {
        ArrayList<Integer> t;
        if(optAccesses.containsKey(a.address))
        {
          t = optAccesses.get(a.address);
          t.add(accesses.indexOf(a));
          optAccesses.replace(a.address, t);
        }
        else
        {
          t = new ArrayList<Integer>();
          t.add(accesses.indexOf(a));
          optAccesses.put(a.address,t);
        }
        //System.out.println("Address: " + a.address + " Accesses: " + t.toString());
      }
    }

    boolean found;

    for(memoryAccess ma : accesses)
    {
      /*for(frame f : physicalMemory)
        System.out.print("["+f.virtual_address + "," + f.counter + "]" + " ");
      System.out.println();*/

      calcRefresh(ma.cycles);
      ticks+=ma.cycles;
      found = false;
      if(pageTable.containsKey(ma.address))
      {
        found=true;
        if(ma.operation == 's')
          for(frame f : physicalMemory)
            if(f.virtual_address == ma.address)
            {
              f.dirty=true;
              break;
            }

      }

      if(!found)
      {
        //System.out.println("Fault");
        faults++;
        if(physicalMemory.size() < frames)
        {
          physicalMemory.add(new frame(physicalMemory.size(), ma.address, false, 's'==ma.operation));
          pageTable.put(ma.address, Integer.valueOf(physicalMemory.indexOf(ma.address)));
        }
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
          pageTable.remove(physicalMemory.get(victim).virtual_address);
          physicalMemory.set(victim, new frame(victim, ma.address, false, 's'==ma.operation));
          pageTable.put(ma.address, physicalMemory.indexOf(ma.address));
          //System.out.println("Page op: " + ma.operation + " correct: " + physicalMemory.get(victim).dirty);
        }
      }

      for(frame f : physicalMemory)
        if(f.virtual_address == ma.address)
        {
          f.counter = f.counter | (0x1 << 31);
          break;
        }

      ticks++;
      count++;
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
    frame evict = null;
    int time = -1;
    boolean finished = false;
    for(frame f : physicalMemory)
    {
      finished = true;
      for(Integer v : optAccesses.get(f.virtual_address))
      {
        if(v > count && v < time)
        {
          finished = false;
          break;
        }
        else if(v > count && v > time)
        {
          time = v;
          evict = f;
          finished = false;
          break;
        }
      }

      //System.out.println("in opt: " + f.virtual_address + " " + optAccesses.get(f.virtual_address).toString() + " count = " + count + " time = " + time);

      if(finished)
        return physicalMemory.indexOf(f);

    }
    //System.out.println();
    //System.out.println();
    return physicalMemory.indexOf(evict);
  }

  public static int aging()
  {
    int min = physicalMemory.get(0).counter;
    frame evict = null;
    for(frame f : physicalMemory)
    {
      if(f.counter < min)
      {
        evict = f;
        min = f.counter;
      }
      //System.out.println("f.counter " + f.counter + " min: " + min);
    }

    return physicalMemory.indexOf(evict);
  }

  public static void shift()
  {
    for(frame f : physicalMemory)
    {
      f.counter=f.counter>>1;
      if(f.dirty)
      {
        f.dirty = false;
        writes++;
      }
    }
  }

  public static void calcRefresh(int cycles)
  {
    for(int i = ticks; i < ticks+cycles; i++)
      if((i-1)%refresh==0)
        shift();
    //from count before adding to count after adding if count-1%refresh==0 do a refresh;
  }

}
