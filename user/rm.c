#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


// rm之后，inode结构体里的link计数减少，只有减少到0，才会释放这块磁盘空间
// 否则还可以通过其他link来访问这块数据
// 写时会copy   COW
int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    fprintf(2, "Usage: rm files...\n");
    exit(1);
  }

  for(i = 1; i < argc; i++){
    if(unlink(argv[i]) < 0){
      fprintf(2, "rm: %s failed to delete\n", argv[i]);
      break;
    }
  }

  exit(0);
}
