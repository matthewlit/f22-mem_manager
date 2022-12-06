#ifndef VMM_H
#define VMM_H

#include "interface.h"

typedef struct PAGE
{
    struct PAGE* next;
    void* addr;
    int vp_num;
    int pf_num;
    int ref_bit;
    int mod_bit;
    int marked;
}PAGE;

typedef struct PAGE_TABLE
{
    PAGE* head;
    PAGE* tail;
    int size;
    int num_of_pages;
}PAGE_TABLE;

typedef struct MEM_MANAGER
{
    enum policy_type policy;
    void* vm;
    int vm_size;
    int page_size;
    PAGE_TABLE* page_table;
}MEM_MANAGER;

void new_mem_manager(enum policy_type policy, void *vm, int vm_size, int num_frames, int page_size);
void handler(int sig, siginfo_t* info, void* ucontext);
void push(PAGE* page);
PAGE* pop();
PAGE* find(int vp_num);

#endif
