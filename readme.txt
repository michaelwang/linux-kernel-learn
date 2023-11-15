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




