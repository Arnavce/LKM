#include<sys/socket.h>
#include<linux/netlink.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>

#define NETLINK_USER 31

struct sockaddr_nl src_addr, dest_addr;
struct nlmsghdr *nlh = NULL;
struct iovec iov;
int sock_fd;
struct msghdr msg;

int main() {
    sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_USER);
    if (sock_fd < 0) {
        perror("socket creation failed");
        return -1;
    }

    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = getpid(); // user-space pid

    if (bind(sock_fd, (struct sockaddr *)&src_addr, sizeof(src_addr)) < 0) {
        perror("bind failed");
        close(sock_fd);
        return -1;
    }

    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = 0;   // kernel
    dest_addr.nl_groups = 0;  // unicast

    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(1024));
    memset(nlh, 0, NLMSG_SPACE(1024));
    nlh->nlmsg_len = NLMSG_SPACE(1024);
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;

    strcpy(NLMSG_DATA(nlh), "Hello Kernel!");

    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;
    msg.msg_name = (void *)&dest_addr;
    msg.msg_namelen = sizeof(dest_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    printf("Sending message to kernel...\n");
    sendmsg(sock_fd, &msg, 0);

    recvmsg(sock_fd, &msg, 0);
    printf("Received message from kernel: %s\n", (char *)NLMSG_DATA(nlh));

    close(sock_fd);
    return 0;
}
