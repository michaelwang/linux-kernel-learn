# example for build customize module 
   touch hello-1.c 
   touch Makefile
   sudo make  # build hello-1.c to be the kernel module
   #  query above module if exist , there is nothong expect   
   lsmod |grep hello
   # load hello module into kernel 
   sudo insmod hello-1.ko
   # query the module again
   lsmod |grep hello
   # delte the module 
   sudo rmmod hello-1.ko
   # check the module again 
   lsmod |grep hello 
   rmmod hello_1 
   sudo insmod hello-1.ko

# build
## init_module()とclean_moduleはmodule hookの方法である

## In order to build customize module , since kernel 2.3 , kbuild was introduced.

##　Ubuntu22.04LTS中に、eamcsのeglotのclangdで開発環境が作る
1. sudo apt-get install clangd-15
2. sudo update-alternatives --install /usr/bin/clangd clangd /usr/bin/clangd-15 100
3. eglotのconfigが下記によって追加する
```
(use-package eglot  
  ;; configure clangd for c++ and c
  (when-let* ((clangd (seq-find #'executable-find '("clangd")))
    ;; this has to match the tool string in compile-commands.json
    ;; clangd will then use these tools to get system header paths
              (init-args "--enable-config"))
    (add-to-list 'eglot-server-programs
                 `((c++-mode c-mode) ,clangd ,init-args)))

  :hook ((c-mode) . eglot-ensure)
)
```
4. clangdは他のライブが探示しために、上の```--enable-config```のconfigが必要します、したがって、
下記のファイルはプロジェクトのディレクトリにおいて必要します
```
CompileFlags:   
  Add: [-xc++, -std=c++17, -Wall, -Wno-missing-prototypes,-I/usr/src/linux-headers-5.15.0-84/include]

```

5. add `hello-2.c` file ,
use module_init and module_exit for module init and clean

6. add `hello-3.c` file
__initdata は　Linux macroです、それの機能はデータが初期化するのことである。

7. add `hello-4.c` file
モージュルのコメントが追加するの方法が紹介すること。

8. add `hello-5.c` file
8.1 モージュルにパラメータが渡すの方法が紹介：
 sudo make all 
 sudo insmod hello-5.ko mystring="bebop" myintarray=-1 
 sudo dmesg -t | tail -7 
 sudo rmmod hello-5 
 sudo insmod hello-5.ko mystring="supercalifragilisticexpialidous" myintarray=-1,-1
 sudo dmesg -t | tail -7 
 sudo rmmod hello-5 
 sudo dmesg -t | tail -1 
 sudo insmod hello-5.ko mylong=hello
8.2  どうか複数ファルでモージュルになるの方法が紹介する

```sh
touch start.c 
touch stop.c
```

```Makefile
 obj-m += startstop.o
 startstop-objs := start.o stop.o
```

```sh
 sudo make all 
 sudo insmod startstop.ko 
 sudo dmesg | tail -7 
 sudo rmmod startstop.ko  
 sudo dmesg | tail -7 
```

9. avaliable functions to the module (5.2)
hello.cファイルが追加する、その目的は、モージュル中に、
kernel中の関数だけ使えることが説明する。
gcc -Wall -o hello hello.c
strace ./hello 

10.
(5.3) User Space and Kerner Space
kernel manages all of the hardware resources, such as memory, disk,cpu and so on.
kernelは全部のハードウェアを管理することがあります、例えば、メモーリ、ディスク、cpuなど。

if the user program want to operate with these hardware, it's need
to call the kernel api or use the thirdparty libirary.
もし、ユーザーはハードウェアが操作することがあります、kernelのシステムのAPIとか、
他のライブが使用することが必要あります。

the kernel is in charge of keep all system api call is in order and safe.
kernelがシステムのCALLの順番、安全性がkeepする。

(5.4) Namespace 

In large project, it is nessary to keep global virable in good state,
such as it is can be understand easily by other people, and also need
to create a mechnise to easy create global virable.So there is need
to create namespace between different module.
In linux kernel, there are two methods to do this,the
this first one is to declare all the module virable in a static file.
the second method is to declare a symbole table and register in the
kernel. 

巨大のプロジェクト中に、変数の良く状態がくらせるようにすることがある。
全局の変数は容易に理解するできるようにとか、容易に新しい全局の変数作成するように、
namespaceが導入することが必要ある。
その目的になってように、２つ方法がある、一つの方法は

(5.5) Code Space
Each process has seperate memeory space, eash process can only
access it's own memory space. segment error will occure,
if the pointer access a address that does not exist.

If segment error happend in the module which load by the kernel,
the kernel will also crash, because the module share the same
memory space with the kernel. 

But there are also some microkernel, which the module will have their
only memeory space.

各プロセスは独立のメモーリ区間があります、
各プロセスは自身のメモーリ区間がだけアクセスできる。
もし、コードのpointerがメモーリの存在がなくアドレスセグに指摘した時に、
メントエラーが発生ことがある。

モージュルとkernelは同じくメモーリ区間が共通ことがあるので、
もし、kernelに負担されたモージュルが存在がなくアドレス指摘時に、kernelは失敗ことがある。
ただ、microkernelがある、それとモージュルのメモーリ区間が共通がない。

5.6 (Device Drivers)

All the device are organized in the /dev directory,
we also can manage the device in the directory, but
we need to tell driver where to load them. If the sound
device driver is es1370.ko , it will read the device from
the file /dev/sound, and the /dev/sound file actual represents 
the hardware is Ensoniq IS1370 sound card, the user space program
mp3blaster is reading and write throuth the file /dev/sound
which did not need to know which device is installed.

In linux kernel, the device is divided by
character device and block device,block device
can only send and receive information in block.

however the character device can only send and receive the
information by character, we can use `ls -l` command to tell
if the device is character or block device,for example:

全部のディバスは/devのディレクトリ中に保存する。ただ、私たちがディバスに
他のディレクトリ中に保存させることができる、しかし、ディバスファイルが
どこうに保存させるのはdriverに伝えられる必要ことがある。 声カードは/dev/sound
ふぁいであり、声カードのハードウェアはEnsoniq IS1370がである、声カードのdriverが
/dev/soundファイルが対する処理必要だけことである、どの具体的にハードウェアがインストール
ことが知るの必要ない。

Linux中に、ブロクのディバスと文字のディバス２つの種類ディバスがある。文字のディバスが
文字の情報処理することがだけできる。ただ、ブロクのディバスがブロクの情報が処理するだけ
できる。

ディバスの型の情報取得するのは`ls -l`コマンドが実行する必要ことがある。
一つの例の文字は`b`が表示する時に、ブロクのディバスの意味である、ただ、
一つの例の文字は`c`が表示する時に、文字のディバスの意味である。



```sh
michael@mars:~/.../hello-1$ ls -l /dev/sda /dev/sdb 
brw-rw---- 1 root disk 8,  0 11月  3 16:40 /dev/sda
brw-rw---- 1 root disk 8, 16 11月  3 16:40 /dev/sdb
```
the first character of the first column is `b` which means
block device.

```sh
michael@mars:~/.../hello-1$ ls -l /dev/tty* 
crw-rw-rw- 1 root    tty     5,  0 11月  8 18:57 /dev/tty
crw--w---- 1 root    tty     4,  0 11月  3 16:40 /dev/tty0
crw--w---- 1 root    tty     4,  1 11月  3 16:41 /dev/tty1
crw--w---- 1 root    tty     4, 10 11月  3 16:40 /dev/tty10
```
the first character of the column is `c` which means character
device.The column which speperate by the `,` means device type,
the first number is the device number which recognized by the
kernel, and the second number which is recognized by the device
driver. 

6 Character
6.1 character device
In linux kernel, it provides standard operation for communicate
with the character hardware.The standard operation file is under
https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/tree/include/linux/fs.h,
but the driver does not need to implement all those functions,
for example the vedio driver doese not need to implement the directory
operations. The not implement operations will be set null for nessary.
There is some convinient ways to define struct.
There are two methods to do this thing, such as the gcc extension and C99,
detail information about how to do this , can read the below reference.
P27 and  [gcc](https://gcc.gnu.org/onlinedocs/gcc/Designated-Inits.html).
And the designated-Inits is the prefered way to do this.
But in Linux kernel 5.4, an new way was introduced : proc_ops

Kerneln中に、characterのディバスに標準の操作がある。それは
https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/tree/include/linux/fs.h,
Driverが全部の操作関数が実装することが必要がない。
それは実装しないの方法はNULLに割り当てることがあります。
その標準のstructが簡単に実装することができる、２つの方法がある。
まずの方法はGCCの拡大です。他の方法はgccのdesignated-initsです。それの具体的の
使用資料はP27と[gcc](https://gcc.gnu.org/onlinedocs/gcc/Designated-Inits.html)に
読んでことができる。

(6.2)
The file structure
The device in the kernel is represented as file which is defined by include/linux/fs.h,
the file can not been seen by program which is run in the user space.
The normal file which is defined by the glibc which is can noly been seen by the program
in the user space and can not been seen from the kernel.
The device file content is not read by the driver, and the driver only read the struct which
driver will the the struct eleswhere, and the device file type is inode, in program we usually
call it `filp`.

ディバスがkernel中にファイルに表示された、このファイルが include/linux/fs.hで新規することがあります。
しかし、このファイルがユーザーの空間のプログラミングが見つかれない。
普通のファイルがGLIBCで新規することがあります、このファイルがkernel中に見つかれない。
ディバスのファイルの内容がdriverが読んでない、ただ、このファイルが中にstructのデータが読んで他の所に
しようされる。このファイルの型はinodeである。

(6.3)
Register Character Device
There are two ways to register driver into kernel,
the first one is using below function
```c
int register_chrdev(unsigned int major, const char *name, struct, file_operations *fops);
```
the `const char *name` will be displayed in `/proc/devices`, if you did not the major number
you can pass 0 into the function as `unsigned int major`, and the kernel will assign  a
dynamic major number to you as the function's return value.
The downside of this method is that you can not create the device in advance.
The second way to create device is first to create `struct cdev` instance, and then use below
method
```c
void cdev_init(struct cdev *cdev, const struct file_operations *fops);
```
to initialize the method and the below method to add the device to the kernel
```c
int cdev_add(struct cdev *p, dev_t dev, unsigned count);
```

新規のディバスがkernelに登録されることが２つ方法がある。
まずは、次の関数が使用することができる
```c 
void cdev_init(struct cdev *cdev, const struct file_operations *fops);
```
この関数は短所のことがある、それはディバス先に登録されることができない。
他の方法は`struct cdev`の型を使用することができる、
cdevは実例が先にすることが必要です、後で、次の方法が使用する必要ことがあります。
```c
void cdev_init(struct cdev *cdev, const struct file_operations *fops);
```
to initialize the method and the below method to add the device to the kernel
```c
int cdev_add(struct cdev *p, dev_t dev, unsigned count);
```
cdev_initはディバスが初期化するの目的である、
cdev_addはkernel中に追加するの目的である。


(6.4)
`rmmod` can be used to remove the module from kernel,
however the root are not allowed to do what they want,
if the root want to delete the modle from kernel but it is
being used by other process, the error will occure.
So in the kernel there are some mechnism to prevent this
error situation.
Kernel tract every module usage by count number, we can use
`sudo lsmod` and `cat /proc/modules` to find the number,
the second column is the number which is tracted by the kernel.

`rmmod`はモージュルの削除するコマンドである、ただ、
rootはモージュルに随分に削除することができない。
モージュルは他のプログラミングに使われることがありますので。
kernelはモージュルの使用する数が記録することがあります。
この数は次のコマンドが探すことができる。
```SH
sudo lsmod
```

```sh
cat /proc/modules
```

(6.5)
This chapter is something about how to create device.
There are something need to be watched out.
1. In order prevent different process to open the same device in
the same time, this chapter introduced lock mechnisem for device
open and close. If the device is read by a process, the device
is locked, and after the process is read over the device
lock is released.
2. The command `cat /dev/deviceName` can be executed
when user want to read the device. The device need to register
a read hook in the kernel, in order to response to such read operation.
3. Because if the user want to read the content from device, the output
content is need to be taken from the kernel to the user space, so we
need to use system call to finish this task.
the c file is chardev.c.

この部分の内容はどうかディバスが新規することが説明しました。
新しいディバスが新規する時に、注意事項があります。
1. 同じくディバスが複数のプロセスが同時にアクセスすることが防ぐために、
ロークが使われる必要ことがあります。もし、ディバスがあるプロセスが使用
されることがあれば、ロークが使われることがあります。その後で、このプロセス
がディバスのロークがリリースすることが必要ことがあります。
2.ユーザーはディバスの内容が読んでが欲しいときに、
コマンド`cat /dev/deviceName`が使われる必要ことがあります。
ディバスのモージュル中に、システムの針が必要実装することがあります。

(6.6) Writing Moduls for Multiple Kernel Versions
we can use macro for finding the exact kernel version if you
want to implement module for different version kernel .

7 The /proc File System
There is anothe way for kernel or device to send information
to the process which it is the /proc filesystem. Unlike the
real filesystem, the /proc filesystem is a memory index filesystem,
and each file's inode information contains the file permission,
device name.
Also there are some system hook for the device to register if the
device module wants to be communicate using the /proc way.

kernelとディバスがプロセスに通信するの方法は他の方法があります、これは/procがです。
普通のファイルのシステムが異なりますことがあります。/procはメーモンリのファイルシステムである。
毎のinodeの情報はファイルの特権とファイルの内容が含むことがです。

7.1 The proc_ops structure 
The proc_ops which is introduced from Linux v5.6+ ,
and it is the replacement for file_struct_op. In VFS
the  file_struct_op struct has too many field to fill,
and the code may be bloated, the proc_ops struct is the
replacement for the file_stuct_op, and is also has better
performance.

proc_opsはkernel中に、新しいファイル関する操作方法である。
この方法は良い性能がprovideすることがあります。

7.2 Read and Write /proc file
In Linux each process has it's own speperate memeory space,
so if the process need to write data to kernel or the kernel
need to write data to the user process we need to use below
methods,
get_user() and copy_from_user() are methods to read data from
user process.
copy_to_user() and put_user() are methods to write data to
user process.
the process need to pass the memory address which is the
relative address to the segment.So the kernel need to use
above method to get the user process memory segment.

Linux中に、毎のプロセスは独立のメーモンリ空間があります。
そして、もしkernelGはユーザーのプロセスが通信することが
必要あれば、システムのAPIは使用されることがなければならない。
get_user()とget_from_user()はkernelにユーザーのプロセスから
ディターが読んで必要時に使用されるである。
put_user()とcopy_to_user()はkernelから、ユーザーのプロセス
のメーモンリの中に、ディターが書きされる時に使用されるである。


## 7.3
Linux provide way to register file system.Each file system
has it's own way to handle file operaiton and inode operation.
There is a struct inode_operation which hold a pointer to all
of above functions.the innode_operation include pointer which
to struct proc_ops, the proc_ops struct hold function such as
procf_read() and procfs_write().

The write and read role are reversed in kernel, because when the
user want read something from the kernel, and the kernel need to
write it out to the user space. when the user want to write something
to the kernel, the kernel need to read the content from the user space.


The difference between file operation and inode operation is that
file operation only deal with the file content itselft, but the
inode operation operate the file reference information, such as
the file link.

There is aslo permission function in the /proc file system.
when the user want to read something from /proc filesystem,
the permission will take place.

Linuxシステムはファイルのシステムが新規することができます。
毎のファイルのシステムが独立の方法がinodeとファル対する処理ことが
あります。Pointerがあります全部の処理する関数が対することがあります。

(7.4)
In order to simplify the /proc file process , linux kernel
provide start(), next(), stop() method for reading the /prof file
in easily.

start() method is called when /proc file begin to read,
next() method return non-zero integer when there are still
remaing contents to read.

stop() is call when when next() return null.

/procのファイルが簡単に読むことができるために、
kernelはstart()、stop()、next()関数が提供することがあります。
start()は/procのファイルがはじめに読むときに呼び出される。
next()は非ゼロが戻るときに、意味はファルが他の内容待に読むことが表示される。
stop()はnext()がゼロ戻るときに呼び出されるの方法である。

##8 sysfs Intacting with your module

sysfs allows you to interact with the kernel, this is can be
usefully when you need to debug the module through kernel.

```sh
sudo insmod hello-sysfs.ko

sudo cat /sys/kernel/mymodule/myvariable 
0

michael@mars:~/.../hello-1$ sudo echo "32" > /sys/kernel/mymodule/myvariable 
bash: /sys/kernel/mymodule/myvariable: 許可がありません

michael@mars:~/.../hello-1$ sudo dmesg | tail -10
[21768.837482] hello_sysfs: loading out-of-tree module taints kernel.
[21768.837538] hello_sysfs: module verification failed: signature and/or required key missing - tainting kernel
[21768.838211] mymodule: initialised

```
sysfsはユーザーがモージュルに通信されることができる。
これはモジュールのディバグときにとても便利がことがです。


## 9 Talking To Device Files

In linux we need to write or read data to device,
the control of speeding to write or read is very nessary in talking to device.
the `ioctl` tool is invented for this requirement.
This chapter add `ioctl.c` file for this purpose demonstration.
```
sudo make     
make -C /lib/modules/5.15.0-88-generic/build  M=/home/michael/develop/kernel/hello-1 modules
make[1]: ディレクトリ '/usr/src/linux-headers-5.15.0-88-generic' に入ります
  CC [M]  /home/michael/develop/kernel/hello-1/ioctl.o
  MODPOST /home/michael/develop/kernel/hello-1/Module.symvers
  CC [M]  /home/michael/develop/kernel/hello-1/ioctl.mod.o
  LD [M]  /home/michael/develop/kernel/hello-1/ioctl.ko
  BTF [M] /home/michael/develop/kernel/hello-1/ioctl.ko
Skipping BTF generation for /home/michael/develop/kernel/hello-1/ioctl.ko due to unavailability of vmlinux
make[1]: ディレクトリ '/usr/src/linux-headers-5.15.0-88-generic' から出ます

michael@mars:~/.../hello-1$ sudo insmod ioctl.o  
insmod: ERROR: could not insert module ioctl.o: Invalid module format
```

## System Call
System call is another way to  comnunicate with the kernal.
So far, we have know there are 3 ways to comunicate with the kernel.
If we want to use the system call, we need first find the system call
table, and find the api address which we want to call.
The way to find system call table is a little difficult, because
there are some security factor need to consider by the system.
such as the api address can be randomized by system startup.
The kernel will not export system call table since some version because
of the misuse cases.So in order to call system api, we need to do some
patches in the system.
This chapater the author give us a example to explain how to use system call
to monitor a specific user when he or she is open file in the linux. 


システムのapiはkernelに対する通信する方法が使用される。
今、３つの方法がkernelに対する通信する方法がわかりました。
もし、システムのAPIが呼び出されるが欲しいあれば。



## Blocking Process and threads.
In kernel, we need to setup method to block some
process which is reading a file but the file is reading
by the other process.

Simimarly, it is aslo import to block threads which in
the same module are reading the shard resources but the
resources is opend by other threads.


## 12.1 Mutex

In kernel module we need to setup method to
avoid collisions in different module for
share the same resources.


## 12.2 Spinlock
this thechenology is the code lock the cpu for a while,
the code between the lock will ocupply the CPU 100%.
but this should not be used in a long time.
この技術はCPUにロークされて、ユーザーのコードが
CPUに100%に使用されることがですありますので、
続き時間は短くてほうがいいです。


## Read And Lock
the kernel provides eclusive reading and  writing technology.
It is the special lock of spinlock.It is better to keep
the code as short as possible because it will have bad effect
on user experience.

Question:
What if other process do if the cpu is lock by current process?
Wait? Sleep?


Kernelは専用の読みロークと書きロークを提供することがあります。
これは特別のspinlockの種類である。そして、短い時間が使用することがいいです。
その為に、ユーザーはパソコンが使用するときにユーザーの経験がほうがいいです。


## Atomic Read and write.
In the multiple CPU archeteture, we need to operate the
data in atomic ways.
C11 provide the way to operate data in atomic, but it is
not very work well with the kernel memeory model.

マッチポオCPUの環境中に、ディターが原爆に操作する為に、
原爆の方が使用される必要があります。

## Replacing Print Macros
in early day,  tty originally refers the hardware console,
but now , in Linux it refers the ssh, xterm in X11 system.
So tty pointer refers above things.

以前、ttyはハードウェアのコントロールの指摘しました。
今、TTYは、ssh, xtermのものが指摘します。
そして、ttyの指針は上のものが指摘します。
