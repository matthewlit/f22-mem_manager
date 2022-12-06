#include "vmm.h"

MEM_MANAGER mem_manager;

void new_mem_manager(enum policy_type policy, void *vm, int vm_size, int num_frames, int page_size){

    //Initialize mem_manager
    mem_manager.policy = policy;
    mem_manager.vm = vm;
    mem_manager.vm_size = vm_size;
    mem_manager.page_size = page_size;
    
    //Create page_table
    PAGE_TABLE* page_table = (PAGE_TABLE*) malloc(sizeof(PAGE_TABLE));
    page_table->head = NULL;
    page_table->tail = NULL;
    page_table->num_of_pages = 0;
    page_table->size = num_frames;
    mem_manager.page_table = page_table;

    //Set vm to no access
    mprotect(mem_manager.vm,vm_size,PROT_NONE);

    //Set SIGSEGV handler
    struct sigaction action;
    action.sa_flags = SA_SIGINFO;
    action.sa_sigaction = &handler;
    sigaction(SIGSEGV,&action,NULL);
}

void handler(int sig, siginfo_t* info, void* ucontext){
    PAGE* page;
    int addr;
    void* base_addr;
    int vp_num;
    int offset;
    unsigned int phy_addr;
    int evicted;
    int pf_num;
    int writeback;

    addr = (char*)info->si_addr - (char*)mem_manager.vm;
    vp_num = addr/mem_manager.page_size;
    base_addr = mem_manager.vm+(vp_num*mem_manager.page_size);
    offset = addr-(vp_num*mem_manager.page_size);

    //If not in page table
    page = find(vp_num);
    if(page == NULL){
        //If page table full evict 
        if(mem_manager.page_table->num_of_pages == mem_manager.page_table->size){
            PAGE* evicted_page = pop();
            evicted = evicted_page->vp_num;
            pf_num = evicted_page->pf_num;
            writeback = evicted_page->mod_bit;
            mprotect(evicted_page->addr,mem_manager.page_size,PROT_NONE);
            free(evicted_page);
        }
        else{
            evicted = -1;
            writeback = 0;
            pf_num = mem_manager.page_table->num_of_pages;
        }

        //Create new page
        page = (PAGE*) malloc(sizeof(PAGE));
        page->vp_num = vp_num;
        page->addr = base_addr;
        page->pf_num = pf_num;
        page->mod_bit = 0;
        page->marked = 0;
    }
    //If in page table
    else{
        pf_num = page->pf_num;
        writeback = 0;
        evicted = -1;
    }

    phy_addr = (pf_num*mem_manager.page_size)+offset;
    page->ref_bit = 1;
    page->marked = 0;
    
    //Write
    if(((ucontext_t*)ucontext)->uc_mcontext.gregs[REG_ERR] & 0x2){

        mprotect(base_addr,mem_manager.page_size,PROT_READ | PROT_WRITE);

        if(find(vp_num) == NULL){
            page->mod_bit = 1;
            mm_logger(vp_num,1,evicted,writeback,phy_addr);
        }
        else if(page->mod_bit == 1){
            mm_logger(vp_num,4,evicted,writeback,phy_addr);
        }
        else{
            page->mod_bit = 1;
            mm_logger(vp_num,2,evicted,writeback,phy_addr);
        }
    }
    //Read
    else{
        mprotect(base_addr,mem_manager.page_size,PROT_READ);

        if(find(vp_num) == NULL)
            mm_logger(vp_num,0,evicted,writeback,phy_addr);
        else
            mm_logger(vp_num,3,evicted,writeback,phy_addr);
    }

    //Add to page table
    if(find(vp_num) == NULL)
        push(page);

    return;
}

void push(PAGE* page){
    page->next = NULL;

    if(mem_manager.page_table->head == NULL){
        mem_manager.page_table->head = page;
        mem_manager.page_table->tail = page;
    }
    else{
        mem_manager.page_table->tail->next = page;
        mem_manager.page_table->tail = page;
    }

    mem_manager.page_table->num_of_pages++;
}

PAGE* pop(){
    PAGE* temp_page;

    if(mem_manager.page_table->head == NULL){
        return NULL;
    }
    //If FIFO
    else if(mem_manager.policy == MM_FIFO){
        temp_page = mem_manager.page_table->head;
        mem_manager.page_table->head = temp_page->next;
        mem_manager.page_table->num_of_pages--;        
        return temp_page;
    }
    //If Third Chance
    else if(mem_manager.policy == MM_THIRD){

        //Loop till eviction
        while(mem_manager.page_table->head != NULL){

            temp_page = mem_manager.page_table->head;

            //First chance
            if(temp_page->ref_bit == 1){
                temp_page->ref_bit = 0;
                mprotect(temp_page->addr,mem_manager.page_size,PROT_NONE);
            }
            
            //Second chance
            else if(temp_page->ref_bit == 0){

                //Third chance
                if(temp_page->mod_bit == 1 && temp_page->marked == 0){
                    temp_page->marked = 1;
                }
                //Evict
                else if(temp_page->mod_bit == 0 || temp_page->marked == 1){

                    mem_manager.page_table->head = temp_page->next;
                    mem_manager.page_table->num_of_pages--;        
                    return temp_page;
                }
            }
            
            if(mem_manager.page_table->num_of_pages > 1){
                mem_manager.page_table->head = temp_page->next;
                temp_page->next = NULL;
                mem_manager.page_table->tail->next = temp_page;
                mem_manager.page_table->tail = temp_page;
            }
        }
    }
    return NULL;
}

PAGE* find(int vp_num){
    PAGE* current_page;

    current_page = mem_manager.page_table->head;
    while(current_page != NULL){
        if(current_page->vp_num == vp_num){
            return current_page;
        }
        else{
            current_page = current_page->next;
        }
    }
    return NULL;
}