#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/sysfs.h>

static struct kobject *mymodule;

static int myvariable = 0;

static ssize_t myvariable_show(struct kobject *kobj,
                               struct kobj_attribute *attr,
                               char *buf)
{
        return sprintf(buf, "%d\n", myvariable);
}

static ssize_t myvariable_store(struct kobject *kobj,
                                struct kobj_attribute *attr,
                                char *buf,
                                size_t count)
{
        sscanf(buf, "%u", &myvariable);
        return count;
}

static struct kobj_attribute myvariable_attribute =
        __ATTR(myvariable, 0660, myvariable_show, (void *)myvariable_store);

static int __init mod_init(void)
{
        int error = 0;

        pr_info("initializing module\n");

        mymodule = kobject_create_and_add("mymodule", kernel_kobj);
        if (!mymodule)
                return -ENOMEM;

        error = sysfs_create_file(mymodule, &myvariable_attribute.attr);
        if (error)
                pr_info("failed to create myvariable file\n");

        return error;
}

static void __exit mod_exit(void)
{
        pr_info("exiting module\n");
        kobject_put(mymodule);
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
