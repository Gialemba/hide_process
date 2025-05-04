#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/version.h>
#include <linux/namei.h>

/**/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tali");
MODULE_DESCRIPTION("Kernel Module to hide process");
MODULE_VERSION("1");
/**/

#if defined(CONFIG_X86_64) && (LINUX_VERSION_CODE >= KERNEL_VERSION(4,17,0))
#define PTREGS_SYSCALL_STUBS 1
#endif

#ifdef PTREGS_SYSCALL_STUBS
static asmlinkage long (*orig_getdents64)(const struct pt_regs *);


#endif

static int __init ft_init_module(void)
{
	printk(KERN_INFO "kernel module loaded !\n");
	return (0);
}

static void __exit ft_exit_module(void)
{
	printk(KERN_INFO "kernel module unloaded");
}

module_init(ft_init_module);
module_exit(ft_exit_module);
