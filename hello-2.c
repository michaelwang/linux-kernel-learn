/**
 * hello-2.c - Demonstrating the module_int() and module_exit() macros.
 * This is preferred over using int_module() and cleanup_module().
 */
#include <linux/init.h>   /*Needed for the macros*/
#include <linux/module.h> /*Need by all modules*/
#include <linux/printk.h> /* Needed for pr_info() */

static int __init hello_2_init(void) {
  printk(KERN_INFO "Hello, world 2\n");
  return 0;
}

static void __exit hello_2_exit(void) { pr_info("Goodby, world 2\n"); }

module_init(hello_2_init);
module_exit(hello_2_exit);

MODULE_LICENSE("GPL");
