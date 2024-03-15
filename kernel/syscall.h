// System call numbers
// 这个数字标识左移的位数
// 比如SYS_wait对应的系统调用号的十进制数是8   因为它是1<<3得到的
#define SYS_fork     1
#define SYS_exit     2
#define SYS_wait     3
#define SYS_pipe     4
#define SYS_read     5
#define SYS_kill     6
#define SYS_exec     7
#define SYS_fstat    8
#define SYS_chdir    9
#define SYS_dup     10
#define SYS_getpid  11
#define SYS_sbrk    12
#define SYS_sleep   13
#define SYS_uptime  14
#define SYS_open    15
#define SYS_write   16
#define SYS_mknod   17
#define SYS_unlink  18
#define SYS_link    19
#define SYS_mkdir   20
#define SYS_close   21
// 要为新增加的系统调用定义一个系统调用号
#define SYS_trace   22
#define SYS_sysinfo 23
