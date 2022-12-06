#include "interface.h"
#include "vmm.h"


void mm_init(enum policy_type policy, void *vm, int vm_size, int num_frames, int page_size)
{
    new_mem_manager(policy,vm,vm_size,num_frames,page_size);
}