#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>


#define PROC_NAME "Security_driver_acess"

static char message[32] "Driver iniciado com segurança!\n";
// restante do codigo aqui
