#!/bin/sh

#如果同样是要创建 4G 大小的交换创建文件，快捷创建方式的命令如下：
sudo fallocate -l 4G /swap

#启用Swap分区文件
#我们的 swap 交换文件已经创建好了，但 Ubuntu 14.04 还不知道它是被用作交换分区，所以我们还需要将此文件格式化为 Swap 分区交启用它。

#首先，我们需要使用如下命令更改下 swap 文件的权限，以避免其被无意理性到：

sudo chmod 600 /swap

#然后，我们需要用如下将 swap 初始化为交换文件：

sudo mkswap /swap

#最后，还需要使用如下命令来启用交换文件：

sudo swapon /swap

#配置启用时挂载Swap分区文件

#Swap 交换文件虽然已经配置好并已经启用了，但 Ubuntu 14.04 在重启后是不会挂载我们刚创建的交换分区文件的，因此我们还需要对 /etc/fstab 文件进行更改，让其在系统启动时就自动挂载我们创建的 /swap 文件。

#使用 vi 或 nano 在 /etc/fstab 文件底部添加如下内容：

#/swap none swap sw 0 0
