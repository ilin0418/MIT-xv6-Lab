#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
    uint64 address; // arg 0 the starting virtual address of the first user page to check
    int n; // the number of pages to check 
    int bitmask; // user address that's stored into a bitmask
    int buf = 0; // temporary buffer in the kernel;
    struct proc* p = myproc(); // the current process

    argaddr(0, &address);
    argint(1, &n);
    argint(2, &bitmask);

    if( n > 100 || n < 0) //check if vaid (between 1 and 99 pages)
    {
        return -1;
    }

    pte_t *pte = 0;

    for( int i = 0; i < n; ++i)
    {
        int va = address + i * PGSIZE; //virtual space for each space
        pte = walk(p->pagetable, va, 0); //table entry in virtual address
        if(*pte & PTE_A) //access bit was set
        {
            buf = buf | (1L << i); // we know ith page was accessed
        }
        *pte = (*pte) & ~PTE_A; // reset access bit to zero
    }

    if(copyout(p->pagetable, bitmask, (char*)&buf, sizeof(buf)) < 0) //copy to user space
    {
        return -1;
    }

  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
