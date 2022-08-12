#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int sys_fork(void)
{
    return fork();
}

int sys_exit(void)
{
    exit();
    return 0; // not reached
}

int sys_wait(void)
{
    return wait();
}

int sys_kill(void)
{
    int pid;

    if (argint(0, &pid) < 0)
        return -1;
    return kill(pid);
}

int sys_getpid(void)
{
    return myproc()->pid;
}

int sys_sbrk(void)
{
    int addr;
    int n;

    if (argint(0, &n) < 0)
        return -1;
    addr = myproc()->sz;
    if (growproc(n) < 0)
        return -1;
    return addr;
}

int sys_sleep(void)
{
    int n;
    uint ticks0;

    if (argint(0, &n) < 0)
        return -1;
    acquire(&tickslock);
    ticks0 = ticks;
    while (ticks - ticks0 < n)
    {
        if (myproc()->killed)
        {
            release(&tickslock);
            return -1;
        }
        sleep(&ticks, &tickslock);
    }
    release(&tickslock);
    return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int sys_uptime(void)
{
    uint xticks;

    acquire(&tickslock);
    xticks = ticks;
    release(&tickslock);
    return xticks;
}

int sys_draw(void)
{
    void *buf;
    uint size;

    argptr(0, (void *)&buf, sizeof(buf));
    argptr(1, (void *)&size, sizeof(size));

    char text[] = "\n\
    PPPPPP  RRRRRR    AAA   NN   NN   AAA   VV     VV\n\
    PP   PP RR   RR  AAAAA  NNN  NN  AAAAA  VV     VV\n\
    PPPPPP  RRRRRR  AA   AA NN N NN AA   AA  VV   VV\n\
    PP      RR  RR  AAAAAAA NN  NNN AAAAAAA   VV VV\n\
    PP      RR   RR AA   AA NN   NN AA   AA    VVV\n\n";
    if (sizeof(text) > size)
        return -1;

    strncpy((char *)buf, buf, size);
    strncpy((char *)buf, text, size);
    return sizeof(text);
}
