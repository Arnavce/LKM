
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

/* This function is called when the module is loaded */
static int test_lkm_init(void) {
    printk(KERN_INFO "Inserting our first linux kernel module\n");
    return 0;
}

/* This function is called when the module is removed */
static void test_lkm_exit(void) {
    printk(KERN_INFO "Removing our first linux kernel module\n");
}

/* Macros for registering module entry and exit points */
module_init(test_lkm_init);
module_exit(test_lkm_exit);

MODULE_LICENSE("MIT");
MODULE_DESCRIPTION("Test LKM");
MODULE_AUTHOR("Unmesh");
