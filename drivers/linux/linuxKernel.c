#include <linux/module.h>      // Para trabalhar com módulos do kernel
#include <linux/kernel.h>      // Funções e macros do kernel (ex: pr_info, pr_err)
#include <linux/init.h>        // Macros __init e __exit
#include <linux/lsm_hooks.h>   // Para definir os hooks do Linux Security Module
#include <linux/netlink.h>     // Para comunicação via Netlink
#include <net/sock.h>          // Estruturas de socket (netlink)
#include <linux/skbuff.h>      // Manipulação de pacotes (sk_buff)
#include <linux/sched.h>       // Informções de processos (task_struct)
#include <linux/string.h>      // Funções de string (strcmp, etc)
#include <linux/fs.h>          // Manipulação de arquivos
#include <linux/dcache.h>      // Estruturas de cache de diretório
#include <linux/path.h>        // Estruturas para caminhos de arquivos

#define NETLINK_USER 31  // Identificador arbitrário para o protocolo Netlink

MODULE_LICENSE("GPL");  // Licença do módulo (obrigatório)
MODULE_AUTHOR("Peter Steve");  // Autor
MODULE_DESCRIPTION("LSM + Netlink Antivirus Control");  // Descrição do módulo

static struct sock *nl_sk = NULL;  // Socket Netlink

// Estrutura que representa um processo confiável
struct trusted_process {
    const char* name;  // Nome do processo considerado confiável
};

// Lista de processos confiáveis (que podem escrever sem alerta)
static struct trusted_process trusted_list[] = {
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

// Verifica se o processo é confiável com base no nome
static int is_process_trusted(const char *proc_name) {
    int i;
    for (i = 0; i < TRUSTED_LIST_SIZE; i++) {
        if (strcmp(proc_name, trusted_list[i].name) == 0) {
            return 1;  // Encontrado na lista de confiáveis
        }
    }
    return 0;  // Não confiável
}

// Envia mensagem para o espaço do usuário via Netlink
static void send_netlink_message(const char *msg, u32 pid) {
    struct sk_buff *skb_out;
    struct nlmsghdr *nlh;
    int msg_size = strlen(msg);
    int res;

    skb_out = nlmsg_new(msg_size, GFP_KERNEL);
    if (!skb_out) {
        pr_err("[LSM Antivirus] Falha ao alocar sk_buff\n");
        return;
    }

    nlh = nlmsg_put(skb_out, 0, 0, NLMSG_DONE, msg_size, 0);
    NETLINK_CB(skb_out).dst_group = 0;  // Unicast
    memcpy(nlmsg_data(nlh), msg, msg_size);

    res = nlmsg_unicast(nl_sk, skb_out, pid);
    if (res < 0) {
        pr_err("[LSM Antivirus] Erro ao enviar via Netlink: %d\n", res);
        kfree_skb(skb_out);
    }
}

// Função hook de permissão do inode
static int my_inode_permission(struct inode *inode, int mask) {
    const char *comm = current->comm;  // Nome do processo atual

    // Verifica se o processo não confiável está tentando escrever
    if ((mask & MAY_WRITE) && !is_process_trusted(comm)) {
        char *buf = (char *)__get_free_page(GFP_KERNEL);  // Aloca página para armazenar caminho
        char *path = NULL;

        if (!buf)
            return 0;  // Falha ao alocar, mas não bloqueia

        // Obtém o caminho do diretório atual do processo
        path = dentry_path_raw(current->fs->pwd.dentry, buf, PAGE_SIZE);

        if (!IS_ERR(path)) {
            char msg[256];
            snprintf(msg, sizeof(msg), "ACTION:MODIFY|PROC:%s|PATH:%s", comm, path);
            send_netlink_message(msg, 0);  // Envia para espaço do usuário (PID 0 = broadcast)
            pr_info("%s\n", msg);  // Log no kernel
        }

        free_page((unsigned long)buf);  // Libera memória

        // Pode-se bloquear ou apenas alertar. Aqui só alerta:
        // return -EACCES;  // Para bloquear acesso de escrita
        return 0;  // Permite
    }

    return 0;  // Permite se for leitura ou processo confiável
}

// Hook de exemplo (pode ser expandido para outros eventos LSM)
static int my_lsm_function(...)  // Substituir por parâmetros reais conforme o hook usado
{
    char *buf = (char *)__get_free_page(GFP_KERNEL);
    char *path = NULL;

    if (!buf)
        return 0;

    path = dentry_path_raw(current->fs->pwd.dentry, buf, PAGE_SIZE);

    if (!IS_ERR(path)) {
        char msg[256];
        snprintf(msg, sizeof(msg), "[Antivirus LSM] Processo %s tentou modificar: %s", current->comm, path);
        send_netlink_message(msg, 0);
        pr_info("%s\n", msg);
    }

    free_page((unsigned long)buf);
    return 0;
}

// Lista de hooks do módulo
static struct security_hook_list my_hooks[] = {
    LSM_HOOK_INIT(inode_permission, my_inode_permission),
    // Outros hooks podem ser adicionados aqui
};

// Inicialização do módulo
static int __init my_lsm_init(void) {
    struct netlink_kernel_cfg cfg = {
        .input = NULL,  // Nenhuma entrada esperada por enquanto
    };

    nl_sk = netlink_kernel_create(&init_net, NETLINK_USER, &cfg);
    if (!nl_sk) {
        pr_err("[LSM Antivirus] Falha ao criar socket Netlink\n");
        return -ENOMEM;
    }

    security_add_hooks(my_hooks, ARRAY_SIZE(my_hooks), "my_antivirus_lsm");  // Registra hooks

    pr_info("[LSM Antivirus] Módulo LSM carregado com sucesso\n");
    return 0;
}

// Finalização do módulo
static void __exit my_lsm_exit(void) {
    if (nl_sk) {
        netlink_kernel_release(nl_sk);  // Libera socket Netlink
    }
    pr_info("[LSM Antivirus] Módulo descarregado\n");
}

#define RATE_LIMIT_SECONDS 5
static unsigned long netlinkMessageLink(void) {
    static time_t last_time = 0;
    time_t current_time = time(NULL);
    if(last_time == 0 || (current_time - last_time) >= RATE_LIMIT_SECONDS);

    last_time = current_time;
    // permite ação
    return 1;
} else {
    //tempo maximo indefinido
    return 0;
}
static  int my_bprm_check_security(struct linux_binprm *bprm){
    const char *proc_name = current->comm;  // Nome do processo atual

    if(!is_process_trusted(proc_name)){
        char msg[256];
        snprintf(msg, sizeof(msg), "ACTION:EXEC|PROC:%Security", proc_name, msg, bprm->filename);
        send_netlink_message(msg, 0);  // Envia para espaço do usuário (PID 0 = broadcast)
        pr_info("%s\n", msg);  // Log no kernel
        return -EACESS
    }
    return 0;  // Permite se for um processo confiável
}
return 1; // recusa a permissão 
#define LSM_HOOK_INIT(bprm_check_security, my_bprm_check_security),
// Macro para registrar o hook de verificação de segurança do binário

RESE_SECURITY_HOOK(bprm_check_security, my_bprm_check_security);  // Registra o hook de verificação de segurança do binário
module_init(my_lsm_init);  // Macro de inicialização
module_exit(my_lsm_exit);  // Macro de finalização

MODULE_VERSION("1.0");
MODULE_ALIAS("antivirus_lsm");
MODULE_INFO(vermagic, "1.0");
MODULE_INFO(name, "LSM + Netlink Antivirus Control");
MODULE_INFO(srcversion, "1.0");
MODULE_INFO(depends, "");
