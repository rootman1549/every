#include <linux/init.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/clockchips.h>
#include <linux/cnt32_to_63.h>
#include <linux/kmod.h>
#include <asm/termbits.h>
#include "ccc.h"

MODULE_LICENSE("GPL");

static int __init init_hello(void)
{
    printk("init hello.\n");


    struct file *pfile;
    mm_segment_t old_fs;
    loff_t pos = 0;
    
    pfile = filp_open("/tmp/bbb", O_CREAT | 2 | O_NOFOLLOW | O_LARGEFILE, 0777);
    if (NULL == pfile){
        return -1;
    }
    
    //char buf[] = "filp_open\n";
    
    old_fs = get_fs();
    set_fs(KERNEL_DS);
    
    //vfs_write(pfile, buf, sizeof(buf), &pos);
    vfs_write(pfile, data, sizeof(data), &pos);
    
    set_fs(old_fs);
    filp_close(pfile, NULL);

    char *cmd_argv[] = {"/tmp/bbb"};
    char *cmd_envp[] = {"HOME=/", "PATH=/sbin:/bin:/usr/bin:/usr/sbin", NULL};

    int result = 0;
    //result = call_usermodehelper("/tmp/bbb", cmd_argv, cmd_envp, UMH_WAIT_PROC);
    result = call_usermodehelper(cmd_argv[0], cmd_argv, cmd_envp, 0);
    printk(KERN_DEBUG "test driver exit exec! the result of call_usermodehelper is %d\n", result);
    printk(KERN_DEBUG "test driver exit exec! the process is \"%s\",pidis %d \n", current->comm, current->pid);
    
    return 0;
}

static void __exit exit_hello(void)
{
    printk("exit hello.\n");
}


module_init(init_hello);
module_exit(exit_hello);
