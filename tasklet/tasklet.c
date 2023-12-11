#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/printk.h>

/**
 * Macro DECLARE_TASKLET_OLD exists for compatibility
 */
#ifndef DECLARE_TASKLET_OLD
#define DECLARE_TASKLET_OLD(arg1, arg2) DECLARE_TASKLET(arg1, arg2, 0L)
#endif

static void tasklet_fn(unsigned long data)
{
        pr_info("tasklet function started\n");
        mdelay(5000);
        pr_info("tasklet function ended\n");
}

static DECLARE_TASKLET_OLD(mytask, tasklet_fn);

static int __init example_tasklet_init(void)
{
        pr_info("tasklet example init\n");
        tasklet_schedule(&mytask);
        mdelay(200);
        return 0;
}

static void __exit example_tasklet_exit(void)
{
        pr_info("tasklet example exit\n");
        tasklet_kill(&mytask);
}

module_init(example_tasklet_init);
module_exit(example_tasklet_exit);

MODULE_DESCRIPTION("tasklet example");
MODULE_LICENSE("GPL");
