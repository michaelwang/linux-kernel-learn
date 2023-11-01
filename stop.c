/*
 * stop.c - Illustration of multi field modules
 */

#include <linux/kernel.h> /* We are doing kernel work*/
#include <linux/module.h> /* Specificaly, a module */

void cleanup_module(void) {

  pr_info("Short is the life of a kernel module \n");
}

MODULE_LICENSE("GPL");
