#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/printk.h>

static DEFINE_MUTEX(mymutex);

static int __init example_mutex_init(void)
{
        int ret;

        pr_info("mutex_init\n");

        ret = mutex_trylock(&mymutex);
        if (ret != 0) {
                pr_info("mutex locked\n");

                if (mutex_is_locked(&mymutex) == 0)
                        pr_info("mutex failed to lock!\n");

                mutex_unlock(&mymutex);
                pr_info("mutex unlocked\n");
        } else {
                pr_info("mutex failed to lock\n");
        }

        return 0;
}

static void __exit example_mutex_exit(void)
{
        pr_info("mutex_exit\n");
}

module_init(example_mutex_init);
module_exit(example_mutex_exit);

MODULE_DESCRIPTION("mutex example");
MODULE_LICENSE("GPL");
