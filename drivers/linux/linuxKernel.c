#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/lsm_hooks.h>
#include <linux/netlink.h>
#include <net/sock.h>
#include <linux/skbuff.h>
#include <linux/sched.h>
#include <linux/string.h>


#define NETLINK_USER 31

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Peter Steve");
MODULE_DESCRIPTION("LSM + Netlink Antivirus Control");

static struct sock *nl_sk = NULL;

struct trusted_process {
    const char*name;
};

static struct trusted_process trusted_list[] ={
  {"systemd"},
    {"bash"},
    {"sshd"},
    {"init"},
    {"kthreadd"},
    {"ksoftirqd"},
    {"rcu_sched"},
    {"rcu_bh"},
    {"migration/0"},
    {"watchdog/0"},
    {"kworker/0:0"},
};

#define TRUSTED_LIST_SIZE (sizeof(trusted_list)/sizeof(trusted_list[0]))

static int is_process_trusted(const char *proc_name) {
    int i;
    for (i = 0; i < TRUSTED_LIST_SIZE; i++) {
        if (strcmp(proc_name, trusted_list[i].name) == 0) {
            return 1;
        }
    }
    return 0;
}


static void send_netlink_message(const char *msg, u32 pid) {
    struct sk_buff *skb_out;
    struct nlmsghdr *nlh;
    int msg_size = strlen(msg);
    int res;

    skb_out = nlmsg_new(msg_size, GFP_KERNEL);
    if(!skb_out) {
        pr_err("Failed to allocate new skb\n");
        return;
    }

    nlh = nlmsg_put(skb_out, 0, 0, NLMSG_DONE, msg_size, 0);
    NETLINK_CB(skb_out).dst_group = 0;
    memcpy(nlmsg_data(nlh), msg, msg_size);


    res = nlmsg_unicast(nl_sk, skb_out, pid);
    if(res < 0) {
        pr_err("error while sending back to user\n");
        pr_err("nlmsg_unicast error: %d\n", res);
        kfree_skb(skb_out);
        
    }
}
