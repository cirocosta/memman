
#memman

> A memory management system simulator in C

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

***Table of Contents***

- [Trace](#trace)
- [Prompt](#prompt)
- [Memory Files](#memory-files)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## Trace

```
total_mem virtual_mem
t0 name tf b p1 t1 [pn tn]
...
t0 name tf b p1 t1 [pn tn]
```
where:

- `total_mem`: total physical memory
- `virtual_mem`: virtual memory
- `t0`: time when the process arrives in the system
- `name`: process name
- `tf`: time at which the process is finalized
- `b`: number of bytes utilized by the process
- `pn tn`: position (in local memory) accessed by the process at the `tn` instant of time

## Prompt 

The simulator provides a prompt (`[mm] `) that lets the user insert a few commands:

- `carrega <filename>`: load the given `filename` into the simulator 
- `espaco <number>`: specifies the free space manager algorithm
- `substitui <number>`: specifies the page substitution algorithm
- `executa <interval>`: specifies the "print interval"
- `sai`: finishes the simulation

There are 3 free space management algirhtms:
-  1. First Fit
-  2. Next Fit
-  3. Quick Fit

And four page substitution algorithms:
-  1. Not Recently Used Page
-  2. First-In, First-Out
-  3. Second-Chance Page
-  4. Least Recently Used Page

ps.: assuming that each page and page frame has a size of 16B.

## Memory Files

The simulator creates two binary files (each time they execute), each one simulating a type of memory. They must be initialized with their respective values as specified by the trace file - see [trace](#trace).

- Physical:  `/tmp/mm.mem`
- Virtual:  `/tmp/mm.vir`

All of its positions must be initially intialized with `-1` and later be filled with the corresponding process mem identifier (which must be unique for each process).

## MMU

The MMU is responsible for translating a given virtual addres into a physical address. It does so by mapping the virtual page into a physical page and then passing (without changes) the offset along with the mapped page frame.

```c
v_address = proc_base + proc_address;
phys_addr = MMU(v_address);
```

```c
typedef struct mm_mmu_t {
  VirtualPage* pages;    
  mm_replacement_alg;
} mm_mmu_t;

mm_mmu_t* mm_mmu_create();
mm_mmu_t* mm_mmu_destroy(mm_mmu_t*);

unsigned mm_mmu_map(mm_mmu_t*, mm_segment_t*);
void mm_mmu_unmap(mm_segment_t*);
unsigned mm_mmu_access(mm_mmu_t*, unsigned position);

typedef struct mm_vpage_t {
  char r : 1;
  char m : 1;
  char p : 1;
  unsigned phys_pos;
} mm_vpage_t;
```

## Flow of Execution

1.  Processes come into the system as specified by the traces input file 
  1.1 `mm_manager_t` created with simulation info

2.  Whenever a process comes, memory is assigned (mapped) to the process in the virtual memory. 
  2.1.  `mm_seglist_add_process(process)`: here is where free space management algorithms takes place. A segment is returned, representing the position in virtual memory that the process owns (with `base` and `length`)
  2.2. `mm_mmu_map(base, length)`: let the mmu know about the presence of the process in virtual memory.
    -  All of the virtual memory that the process now owns is marked as not present in the physical memory (indicated by the `P` bit in the virtual page trable's entry).
  2.3. `mm_memory_assign(virtual, start, end, value)` reflects the memory representation in the memory file

3.  Whenever the process tries to access a given memory, MMU deals with it by managing the pages as it needs to (causing page faults when necessary and using the given page replacement algorithm).
  3.1. `mm_mmu_access(position)`

4.  When `tf` comes, i.e, the termination time is reached, physical memory is unmapped if there are any mapping to the process' pages and the free memory management algorithm takes place again to now create a free space and completly remove the process from the virtual memory.


# LICENSE

MPLv2

