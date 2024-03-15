#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "sysinfo.h"
#include "proc.h"

// sysproc.c    sysfile.c    proc.c这些文件基本都是系统调用的具体实现
// 大概按照系统调用的功能区别划分了一下文件
uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
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
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
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

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
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

// click the sys call number in p->tracemask
// so as to tracing its calling afterwards


// trace系统调用在内核中的具体实现，这里就是本lab的主要任务，即实现sys_trace这个系统调用
// 该文件里的其他sys开头的函数，也都是各个系统的具体实现
uint64
sys_trace(void) {

  // 此处的trace_sys_mask传参的方式：使用PCB里的成员变量
  int trace_sys_mask;
  if (argint(0, &trace_sys_mask) < 0)
    return -1;
  // 修改该进程的PCB结构中的tracemask成员
  myproc()->tracemask |= trace_sys_mask;
  return 0;
  
}


// collect system info
// 这是本lab的第二个任务，该系统调用帮助用户访问内核中的sysinfo结构体，获知当前的剩余可用内存，正在运行的进程数
// 将这两个内核变量拷贝到用户空间
uint64
sys_sysinfo(void) {
  // 此处虽然已经变成内核态了
  // 但是程序本身PID和启动这个系统调用的用户进程还是相同的
  // 即只是程序状态和CPU状态的变化，并不是进程的切换
  // 所以此处调用myproc获取的进程pid就是刚才启动这个系统调用的用户进程
  // 但是多核处理器上可以运行多个内核态程序，他们对应多个用户进程
  // 这也是为什么访问内核中的某些数据结构（比如PCB数组）时需要上锁了
  struct proc *my_proc = myproc();
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;


  // 创建一个结构体，用来状态想要获取的两个内核数据
  struct sysinfo s;
  // 获取这两个内核数据：freemem 和 nproc
  s.freemem = kfreemem();
  s.nproc = count_free_proc();
  
  // 将想要获取的内核变量拷贝到用户空间，即将s结构体里的数据拷贝到p指针所指向的内存
  // 这个虚拟地址p在copyout函数中会借助用户进程页表my_proc->pagetable来转换出物理地址
  if(copyout(my_proc->pagetable, p, (char *)&s, sizeof(s)) < 0)
    return -1;
  return 0;
}
