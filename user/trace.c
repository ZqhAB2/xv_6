#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// 该文件是
int
main(int argc, char *argv[])
{
  int i;
  char *nargv[MAXARG];

  if(argc < 3 || (argv[1][0] < '0' || argv[1][0] > '9')){
    fprintf(2, "Usage: %s mask command\n", argv[0]);
    exit(1);
  }

  if (trace(atoi(argv[1])) < 0) {
    fprintf(2, "%s: trace failed\n", argv[0]);
    exit(1);
  }
  
  // 调用时：格式为$trace 32 grep hello README  
  // 从输入参数中解析出要启用的进程名，要追踪的系统调用是32号，即read系统调用
  // 追踪的用户进程名是grep，exec(nargv[0], nargv);中的nargv[0]就是grep   nargv是grep的参数hello README
  // exec相当于在当前的进程中启动了grep进程，然后对其进行系统调用追踪 
  for(i = 2; i < argc && i < MAXARG; i++){
    nargv[i-2] = argv[i];
  }

  exec(nargv[0], nargv);
  exit(0);
}
