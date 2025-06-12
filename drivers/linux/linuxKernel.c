#include <linux/module.h>      // Para módulos do kernel
#include <linux/kernel.h>      // Para funções e macros do kernel
#include <linux/init.h>        // Para macros __init e __exit
#include <linux/lsm_hooks.h>   // Para Linux Security Module (LSM) hooks
#include <linux/netlink.h>     // Para comunicação Netlink
#include <net/sock.h>          // Para estruturas de socket
#include <linux/skbuff.h>      // Para sk_buff (gerenciamento de pacotes)
#include <linux/sched.h>       // Para informações de processos (task_struct)
#include <linux/string.h>      // Para funções de string (strcmp, etc.)

 // Número arbitrário para identificação do socket Netlink
#define NETLINK_USER 31         

MODULE_LICENSE("GPL");          // Licença GPL (obrigatória para módulos do kernel)
MODULE_AUTHOR("Peter Steve");   // Autor do módulo
MODULE_DESCRIPTION("LSM + Netlink Antivirus Control");  // Descrição
static struct sock *nl_sk = NULL;  // Socket Netlink

struct trusted_process {
    const char* name;  // Nome do processo confiável
};

static struct trusted_process trusted_list[] = {
    {"systemd"},        // Processos considerados confiáveis
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

#define TRUSTED_LIST_SIZE (sizeof(trusted_list)/sizeof(trusted_list[0]))  // Tamanho da lista

static int is_process_trusted(const char *proc_name) {
    int i;
    for (i = 0; i < TRUSTED_LIST_SIZE; i++) {
        if (strcmp(proc_name, trusted_list[i].name) == 0) {
            return 1;  // Processo encontrado na lista de confiáveis
        }
    }
    return 0;  // Processo NÃO está na lista
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
#include <linux/fs.h>
#include <linux/dcache.h>
#include <linux/path.h>


static int my_inode_permission(struct inode *inode, int mask) {
const char *comm = current->comm;

    // Verifica se está tentando escrever (ou criar)
    if ((may & MAY_WRITE) && !is_process_trusted(comm))
        char get_free_page(GFP_KERNEL);
        char *path = NULL;
    
}

static int my_lsm_function(...)  // Substitua pelos parâmetros corretos
{
    char *buf = (char *)__get_free_page(GFP_KERNEL);
    char *path = NULL;

    if (!buf)
        return 0;  // não bloqueia se falhar

    path = dentry_path_raw(current->fs->pwd.dentry, buf, PAGE_SIZE);

    if (!IS_ERR(path)) {
        char msg[256];
        snprintf(msg, sizeof(msg), "[Antivirus LSM] Processo %s tentou modificar: %s", comm, path);
        send_netlink_message(msg, 0);  // pid = 0 (broadcast)
        pr_info("%s\n", msg);
    }

    free_page((unsigned long)buf);

    return 0; // permitir por enquanto
}
static struct security_hook_list my_hooks[] = {
    LSM_HOOK_INIT(inode_permission, my_inode_permission),
    // Adicione outros hooks conforme necessário
};


static int __init my_lsm_init(void) {
struct netlink_kernel_cfg cfg = {
    .input = NULL,
} 
nl_sk = netlink_kernel_create(&init_net, NETLINK_USER, &cfg);
   if(!nl_skj) {
    pr_err("Netlink Kernel create failed\n ");
    return -ENOMEM;  // Falha ao criar socket Netlink
   }


   security_add_hooks(my_hooks, ARRAY_SIZE(my_hooks), "my_antivirus_lsm");

   pr_info("LSM + Netlink Antivirus Control module Loaded\n");
   return 0;  // Sucesso
}  



     static void __exit my_lsm_exit(void) {
        if (nl_sk) 
             netlink_kernel_release(nl_sk);
             pr_info("LSM + Netlink Antivirus Control module UnLoded\n");
     }

     module_init(my_lsm_init);  // Função de inicialização do módulo
     module_exit(my_lsm_exit);  // Função de limpeza do módulo

MODULE_VERSION("1.0");  // Versão do módulo
MODULE_ALIAS("antivirus_lsm");  // Alias para o módulo
MODULE_INFO(vermagic, "1.0");  // Informações de versão do módulo
MODULE_INFO(name, "LSM + Netlink Antivirus Control");  // Nome do módulo
MODULE_INFO(srcversion, "1.0");  // Versão do código-fonte
MODULE_INFO(depends, "");  // Dependências do módulo    
