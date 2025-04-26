#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define BUFFER_SIZE 128
#define PROC_NAME "add_process_lkm"

static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);

static struct proc_ops proc_op_0 = {
    .proc_read = proc_read,
};

/* Called when the module is loaded */
static int proc_init(void) {
    proc_create(PROC_NAME, 0, NULL, &proc_op_0);
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
    return 0;
}

/* Called when the module is removed */
static void proc_exit(void) {
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

/* Read function for /proc/add_process_lkm */
static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos) {
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;

    if (completed) {
        completed = 0;
        return 0;
    }

    completed = 1;
    rv = sprintf(buffer, "A process has been added to the pseudo file system\n");
    copy_to_user(usr_buf, buffer, rv);

    return rv;
}

/* Module entry and exit macros */
module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("MIT");
MODULE_DESCRIPTION("Add Process LKM");
MODULE_AUTHOR("SGG");
