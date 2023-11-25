/**
 * print_string.c - Send output to the tty
 * we're running on, regardless if it is throuth X1,
 * telnet, etc. We do this by printing the string to
 * the tty associated with the current task.
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h> /*For current */
#include <linux/tty.h>   /*For the tty declarations*/

static void print_string(char *str) {
  /*The tty for the current task */
  struct tty_struct *my_tty = get_current_tty();

  /* If my_tty is NULL. the current task has no tty
   * you can print to (i.e, if it is a daemon). If so,
   * there is nothing we can do.
   */

  if (my_tty) {
    const struct tty_operations *ttyops = my_tty->driver->ops;
    /* my_tty->driver is a struct which holds the tty's
     * functions. One of whic(write) is used to write strings to the tty. It can
     * be used to task a string either from the user's or kernel's memory
     * segment.
     *
     * The function's 1st parameter is the tty to write to, because the same
     * function would normally be used for all tty's of a certain type.
     *The 2nd paramter is a pointer to string.
     *The 3rd paramter is the length of the string.
     *
     * As you will see below, sometimes it's
     * necessary to use preprocessor stuff to create
     * code that works for different kernel versions.
     * The (navie) approach we've taken here does not
     * scale well. The right way to deal with this is described in section 2 of
     *linux/Documentation/SubmittingPatches
     */
    (ttyops->write)(my_tty,       /* The tty itself */
                    str,          /*string*/
                    strlen(str)); /*length*/

    /*
     * ttys were originally hardware devices. which (
     * usually) strictly followed the ASCII standard.
     * In ASCII , to move to a new line you need two
     * characters, a carrigage return and a line feed.
     * On unix, the ASCII line feed is used for both
     * purpose - so we can not just use \n, because it
     * would not have a carriage return and the next line will start at the
     * column right after the line feed.
     *
     * This is why text files are different between Unix
     * and MS Windows. In CP/M and derivatives, like MS-DOS and MS Windows, the
     * ASCII standard was strctly adhere to , and therefor a newline requires
     * both LF and a CR.lk
     */
    (ttyops->write)(my_tty, "\015\012", 2);
  }
}
static int __init print_string_init(void) {
  print_string("The module has been inserted. Hello world!");
  return 0;
}

static void __exit print_string_exit(void) {
  print_string("The module has been removed. Farewell world!");
}

module_init(print_string_init);
module_exit(print_string_exit);
MODULE_LICENSE("GPL");
