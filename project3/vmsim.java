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
  public static int refreshCount;

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
    refreshCount=0;
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
      //for(frame f : physicalMemory)
        //System.out.print("["+f.virtual_address + "," + f.counter + "," + f.dirty + "]");
      //System.out.println();

      //System.out.println("Looking for page: " + ma.address);

      if(algorithm.equals("aging"))
        calcRefresh(ma.cycles);

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

    if(algorithm.equals("aging"))
      for(frame f : physicalMemory)
        if(f.virtual_address == ma.address)
        {
          f.reference = 1;
          break;
        }

      count++;
    }
    //System.out.println("Refreshes: " + refreshCount);

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
    int type=0;
    int min = 0xffff;
    frame evict = null;
    do
    {
      for(frame f : physicalMemory)
      {
        if(f.counter <= min && !f.dirty && f.reference==0 && type==0)
        {
          if(f.counter == min)
          {
            if(evict != null)
            {
              if(f.virtual_address < evict.virtual_address)
              {
                evict = f;
                min = f.counter;
                continue;
              }
            }
          }
          else
          {
            evict = f;
            min = f.counter;
          }
        }

        else if(f.counter <= min && f.dirty && f.reference==0 && type==1)
        {
          if(f.counter == min)
          {
            if(evict != null)
            {
              if(f.virtual_address < evict.virtual_address)
              {
                evict = f;
                min = f.counter;
                continue;
              }
            }
          }
          else
          {
            evict = f;
            min = f.counter;
          }
        }

        else if(f.counter <= min && !f.dirty && f.reference==1 && type == 2)
        {
          if(f.counter == min)
          {
            if(evict != null)
            {
              if(f.virtual_address < evict.virtual_address)
              {
                evict = f;
                min = f.counter;
                continue;
              }
            }
          }
          else
          {
            evict = f;
            min = f.counter;
          }
        }
        else if(type == 3)
        {
          if(f.counter == min)
          {
            if(evict != null)
            {
              if(f.virtual_address < evict.virtual_address)
              {
                evict = f;
                min = f.counter;
                continue;
              }
            }
          }
          else
          {
            evict = f;
            min = f.counter;
          }
        }
      }
      type++;
    } while (evict == null);

      //System.out.println("f.counter " + f.counter + " min: " + min);

    //System.out.println("Swapping out page: " + evict.virtual_address);
    //System.out.println();
    return physicalMemory.indexOf(evict);
  }

  public static void shift()
  {
    int r=0;
    for(frame f : physicalMemory)
    {
      r=f.reference;
      f.counter=(f.counter>>1)|(f.reference<<3);
      f.reference=0;
    }
    refreshCount++;
  }

  public static void calcRefresh(int cycles)
  {
    //System.out.println("Begin: Cycles: "+ cycles + " Ticks: " + ticks + " refresh: " + refresh);
    while(ticks-refresh>=0)
    {
      ticks-=refresh;
      shift();
    }
    while(cycles - refresh >= 0)
    {
      //System.out.println("Middle: Cycles: "+ cycles + " Ticks: " + ticks + " refresh: " + refresh);
      cycles-=refresh;
      shift();
    }
    ticks+=cycles;
    if(ticks%refresh==0)
    {
      ticks=0;
      shift();
    }
    //System.out.println("End: Cycles: "+ cycles + " Ticks: " + ticks + " refresh: " + refresh);
  }

}
