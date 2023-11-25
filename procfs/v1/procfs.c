#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define HAVE_PROC_OPS
#endif

#define procfs_name     "procfsv1"

static struct proc_dir_entry *procfile;

static ssize_t procfile_read(struct file *filp,
                             char __user *buf,
                             size_t len,
                             loff_t *off)
{
        char s[13] = "Hello World!\n";
        int ret;

        if (*off >= len || copy_to_user(buf, s, sizeof(s))) {
                pr_info("could not copy to user\n");
                ret = 0;
        } else {
                pr_info("read %s\n", filp->f_path.dentry->d_name.name);
                ret = len;
                *off += len;
        }

        return ret;
}

#ifdef HAVE_PROC_OPS
static const struct proc_ops proc_ops = {
        .proc_read = procfile_read,
};
#else
static const struct file_operatioins proc_ops = {
        .read = procfile_read,
};
#endif

static int __init procfs_init(void)
{
        procfile = proc_create(procfs_name, 0644, NULL, &proc_ops);
        if (procfile == NULL) {
                proc_remove(procfile);
                pr_alert("could not create /proc/%s", procfs_name);
                return -ENOMEM;
        }

        pr_info("/proc/%s created\n", procfs_name);
        return 0;
}

static void __exit procfs_exit(void)
{
        proc_remove(procfile);
        pr_info("/proc/%s removed\n", procfs_name);
}

module_init(procfs_init);
module_exit(procfs_exit);

MODULE_LICENSE("GPL");
