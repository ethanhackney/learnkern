#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/spinlock.h>

static DEFINE_SPINLOCK(sl_static);
static spinlock_t sl_dynamic;

static void spinlock_static(void)
{
        unsigned long flags;

        spin_lock_irqsave(&sl_static, flags);
        pr_info("locked static spinlock\n");
        spin_unlock_irqrestore(&sl_static, flags);
        pr_info("unlocked static spinlock\n");
}

static void spinlock_dynamic(void)
{
        unsigned long flags;

        spin_lock_init(&sl_dynamic);
        spin_lock_irqsave(&sl_dynamic, flags);
        pr_info("locked dynamic spinlock\n");
        spin_unlock_irqrestore(&sl_dynamic, flags);
        pr_info("unlocked dynamic spinlock\n");
}

static int __init example_spinlock_init(void)
{
        pr_info("example spinlock started\n");

        spinlock_static();
        spinlock_dynamic();

        return 0;
}

static void __exit example_spinlock_exit(void)
{
        pr_info("example spinlock exit\n");
}

module_init(example_spinlock_init);
module_exit(example_spinlock_exit);

MODULE_DESCRIPTION("spinlock example");
MODULE_LICENSE("GPL");
