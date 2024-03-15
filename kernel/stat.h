#define T_DIR     1   // Directory
#define T_FILE    2   // File
#define T_DEVICE  3   // Device

// 这就是inode中保存文件元数据信息的结构体
// 包含了文件的：文件类型、文件的link数量、文件的大小、在磁盘上的位置等
// nlink描述有几个别名指向该inode数据


// link的概念：硬链接，目录中的每个条目都是一个link，比如将a文件拷贝一份生成b文件，在写之前
// 这两个文件名是指向磁盘上同一个实际的物理inode的，即b是a的别名      除非写入，才会发生COW，否则只有一块物理数据

struct stat {
  int dev;     // File system's disk device
  uint ino;    // Inode number
  short type;  // Type of file
  short nlink; // Number of links to file
  uint64 size; // Size of file in bytes
};
