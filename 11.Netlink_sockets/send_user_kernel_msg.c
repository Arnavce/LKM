#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <linux/init.h>
#include <net/sock.h>

#define NETLINK_USER 31

struct sock *nl_sk = NULL;

static void recv_msg(struct sk_buff *skb) {
    struct nlmsghdr *nlh;
    int pid;
    struct sk_buff *skb_out;
    int msg_size;
    char *msg = "Hello from Kernel";
    int res;

    nlh = (struct nlmsghdr *)skb->data;
    printk(KERN_INFO "Netlink received msg payload: %s\n", (char *)nlmsg_data(nlh));

    pid = nlh->nlmsg_pid; // PID of sending process
    msg_size = strlen(msg);

    skb_out = nlmsg_new(msg_size, 0);
    if (!skb_out) {
        printk(KERN_ERR "Failed to allocate new skb\n");
        return;
    }

    nlh = nlmsg_put(skb_out, 0, 0, NLMSG_DONE, msg_size, 0);
    strncpy(nlmsg_data(nlh), msg, msg_size);
    ((char *)nlmsg_data(nlh))[msg_size] = '\0';  // ensure null termination

    res = nlmsg_unicast(nl_sk, skb_out, pid);
    if (res < 0)
        printk(KERN_INFO "Error while sending back to user\n");
}

static int __init hello_init(void) {
    struct netlink_kernel_cfg cfg = {
        .input = recv_msg,
    };

    printk(KERN_INFO "Entering: hello_init\n");

    nl_sk = netlink_kernel_create(&init_net, NETLINK_USER, &cfg);
    if (!nl_sk) {
        printk(KERN_ALERT "Error creating Netlink socket.\n");
        return -10;
    }

    return 0;
}

static void __exit hello_exit(void) {
    printk(KERN_INFO "Exiting: hello_exit\n");
    netlink_kernel_release(nl_sk);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
