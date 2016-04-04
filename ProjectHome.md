tiny6410上的superboot的开源实现...
以rt-thread为基础，实现superboot的所有功能...

1、下载工具链，gnu版
在如下网址下载最新的arm工具链
http://www.yagarto.de/

2、编译SDLoader代码，对应为s3c6410启动过程的BL1阶段，烧写目标文件sdloader.bin到sd卡的倒数0x2400位置。

3、编译rt-thread-s代码，对应为s3c6410启动过程的BL2阶段，作为superboot的主体代码，烧写目标文件rtt-tiny6410.bin到sd卡的倒数0x100000位置。

4、插入sd卡上电起动，即进入rt-thread的finsh。


TODO：
1、实现sd卡的驱动
2、实现nand的驱动
3、实现usb驱动
4、。。。。。。