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
