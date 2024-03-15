#!/usr/bin/perl -w

# Generate usys.S, the stubs for syscalls.
# 该文件编译后会生成usys.s文件，该文件中有ecall指令，会实现从用户进程到内核进程的转换

print "# generated by usys.pl - do not edit\n";

print "#include \"kernel/syscall.h\"\n";

sub entry {
    my $name = shift;
    print ".global $name\n";
    print "${name}:\n";
    print " li a7, SYS_${name}\n";
    print " ecall\n";
    print " ret\n";
}

# 一旦检测到这几个函数的名字，就知道是系统调用了
# 就会执行usys.pl的汇编代码
	
entry("fork");
entry("exit");
entry("wait");
entry("pipe");
entry("read");
entry("write");
entry("close");
entry("kill");
entry("exec");
entry("open");
entry("mknod");
entry("unlink");
entry("fstat");
entry("link");
entry("mkdir");
entry("chdir");
entry("dup");
entry("getpid");
entry("sbrk");
entry("sleep");
entry("uptime");
# 新增加的系统调用也要在此处增加entry 
entry("trace");
entry("sysinfo");
