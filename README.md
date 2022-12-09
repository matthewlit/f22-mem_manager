# f22-mem_manager

## About The Project

Memory manager created for CMPSC473, Operating Systems, as project junior year using C in Visual Studio Code. For this project, I implement a memory manager, which manages virtual memory on limited physical memory by implementing an access control mechanism and two page replacement policies, with a given virtual address space and the number of physical pages. Based on the input file, the program reads or writes to the virtual address space, and figures out whether the corresponding memory is in physical memory. If it is, it allow the access to go through, otherwise, it allocate a frame for the virtual page in physical memory and consequently evict some other page as per the page replacement policy if the physical memory is full.

## Input File Format
```
[read|write] [Virtual page number] [Offset] [Value]
```

## Output File Format
```
Page Size: [Page Size]

Num Frames: [Number of Frames]

type    virt-page      evicted-virt-page      write-back     phy-addr

[type]  [virt-page]    [evicted-virt-page]    [write-back]   [phy-addr]

...
```

  * [virt_page]     : Page number of the virtual page being referenced
  * [fault_type]    : Indicates what caused SIGSEGV.
    * 0 - Read access to a non-present page
    * 1 - Write access to a non-present page
    * 2 - Write access to a currently Read-only page
    * 3 - Track a "read" reference to the page that has Read and/or Write permissions on.
    * 4 - Track a "write" reference to the page that has Read-Write permissions on.
  * [evicted_page]  : Virtual page number that is evicted. (-1 in case of no eviction)
  * [write_back]    : 1 indicates evicted page needs writing back to disk, 0 otherwise
  * [phy_addr]      : Represents the physical address (frame_number concatenated with the offset)

## Built With

* Visual Studio Code: https://code.visualstudio.com/

## Contact

Matthew Kelleher:
* Email: matthew.kelleher10@comcast.net
* LinkedIn: https://www.linkedin.com/in/matthew-k-868178128/

Project Link: https://github.com/matthewlit/f22-mem_manager
