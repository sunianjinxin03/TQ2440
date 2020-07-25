# TQ2440
2440project
系统移植出现的问题：启动卡在Freeing init memory: 176K
用 /opt/EmbedSky/4.3.3/bin/arm-none-linux-gnueabi-gcc编译之后系统启动卡在 Freeing init memory: 176K
换 /opt/EmbedSky/crosstools_3.4.5_softfloat/gcc-3.4.5-glibc-2.3.6/arm-linux/bin/arm-linux-gcc
https://blog.csdn.net/wangcong02345/article/details/51810932
https://blog.csdn.net/wangcong02345/article/details/51810932?locationNum=8&fps=1
解决busybox 操作 4.4.3编译器出错

busybox编译
make menuconfig
make 
make CONFIG_PREFIX=../root_fs3 install
CROSS_COMPILE = /opt/EmbedSky/crosstools_3.4.5_softfloat/gcc-3.4.5-glibc-2.3.6/arm-linux/bin/arm-linux-
busubox 编译 error
我们将这些定义放到出错的ionice.c文件中 
#include <sys/syscall.h>
#include <asm/unistd.h>
#include "libbb.h"
#define __NR_ioprio_set              289
#define __NR_ioprio_get              290
#define SYS_ioprio_set __NR_ioprio_set
#define SYS_ioprio_get __NR_ioprio_get
原文链接：https://blog.csdn.net/yikai2009/article/details/8272408

yaffs2最小系统制作编译流程
cd root_fs3 
mkdir -p dev etc home/sky lib mnt opt proc root sys tmp usr/lib usr/share var
mknod console c 5 1
mknod null c 1 3
mknod tq2440_serial0 c 204 64
cp -f /opt/EmbedSky/crosstools_3.4.5_softfloat/gcc-3.4.5-glibc-2.3.6/arm-linux/arm-linux/lib/*.so* lib -a
相关文件都777
https://blog.csdn.net/techhome803/article/details/10136453

制作命令
mkyaffs2image root_fs3  root_fs3.yaffs  //mkyaffs2image为大于128Mnand 工具，2440为256M
chmod 777 root_fs3.yaffs

uboot编译流程
环境变量
echo $KBUILD_OUTPUT
export KBUILD_OUTPUT=./build 
编译 :
make distclean
make tq2440_defconfig
make 
linux编译
make distclean
cp config_EmbedSky_W43 .config
make 
make uImage

打印环境参数
printenv 
分区信息
mtdparts

烧录流程
打开tftpserver软件
烧录NOR
tftp  0x30000000 u-boot.bin
protect off all
erase 0 3ffff
cp.b 30000000 0 40000
cmp.b 0 30000000 40000

烧录NAND
tftp  0x30000000 u-boot.bin
nand erase.part bios
nand write 0x30000000 bios
nand read  0x31000000 bios
cmp.b 0x30000000 0x31000000 40000
Total of 262144 byte(s) were the same

内核下载
tftp  0x30000000 uImage_25
nand erase.part kernel
nand write 30000000 kernel 1d24bc  //2401048

文件系统下载
nand erase.part root
tftp 30000000 root_fs3.yaffs
nand write.yaffs2 0x30000000 root e421c0
help nand 
nand scrub.chip 格式化命令
nand dump 500000 读取500000地址的页内容

原理图信息
NAND flash ： K9F2G08U0B  LDATA0-LDATA15 I/O0-I/O15 没有地址？？？
NOR flash： AM29LV160DB   nGCS0 片选  A0-A21 LADDR1-LADDR22 D0-D15  LDATA0- LDATA15
4Banks x 4M x 16Bit Synchronous DRAM 断电易失设备 RAM： 2 22 * 2    * 4
HY57V561620FTP-H 2片 LnSCS6 片选 地址是LADDR2-LADDR14 A0-A12  片1 DQ0-D015 LDATA0- LDATA15 片2  DQ0-D016 LDATA16- LDATA31
BA0-BA1  LADDR24-LADDR25  / LADDR25-LADDR26  22  存储器 （SROM/SDRAM ） 地址引脚 连接  @ 32 位数据总线 A2,A3...开始
0x3000 0000 - 0x3400 0000  共64M SDRAM 中

