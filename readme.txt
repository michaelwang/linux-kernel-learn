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
