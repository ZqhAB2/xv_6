// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

// 空闲链表上的节点
struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;

void
kinit()
{
  initlock(&kmem.lock, "kmem");
  freerange(end, (void*)PHYSTOP);
}


void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;
  // 如果物理地址超出界限，则报错
  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  // 为希望释放的物理块创建一个节点
  r = (struct run*)pa;  

  acquire(&kmem.lock);
  // 将新释放的节点插到链表头部
  r->next = kmem.freelist;
  // 向前移动链表头部   

   
  kmem.freelist = r;
  release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
// 实现物理页面分配的函数
// freelist就是内存的空闲分区链，每次
void *
kalloc(void)
{
  struct run *r;
  // freelist就是内存的空闲分区链
  acquire(&kmem.lock);
  // 获取链表头节点
  r = kmem.freelist;
  // 如果不为空，就把头节点对应的物理块分配出去，并让头节点指向自己的下一个节点
  if(r)
    kmem.freelist = r->next;
  release(&kmem.lock);

  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}


// 实验2的任务，统计剩余的空闲物理内存
// Return the number of bytes of free memory
// should be multiple of PGSIZE
uint64
kfreemem(void) {
  struct run *r;
  uint64 free = 0;
  acquire(&kmem.lock);
  r = kmem.freelist;
  while (r) {
    free += PGSIZE;
    r = r->next;
  }
  release(&kmem.lock);
  return free;
}
