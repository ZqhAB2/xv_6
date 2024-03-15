#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// 将a文件的物理数据link到别名b上
// 将给a文件增加一个link
int
main(int argc, char *argv[])
{
  if(argc != 3){
    fprintf(2, "Usage: ln old new\n");
    exit(1);
  }
  if(link(argv[1], argv[2]) < 0)
    fprintf(2, "link %s %s: failed\n", argv[1], argv[2]);
  exit(0);
}
