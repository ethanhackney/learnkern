#include <linux/init.h>
#include <linux/module.h>
#include <linux/workqueue.h>

static struct workqueue_struct *queue = NULL;
static struct work_struct work;

static void work_handler(struct work_struct *data)
{
        pr_info("work handler function.\n");
}

static int __init example_workqueue_init(void)
{
        pr_info("example_workqueue_init()\n");
        queue = alloc_workqueue("Hello World!", WQ_UNBOUND, 1);
        INIT_WORK(&work, work_handler);
        queue_work(queue, &work);
        return 0;
}

static void __exit example_workqueue_exit(void)
{
        pr_info("example_workqueue_exit()\n");
        destroy_workqueue(queue);
}

module_init(example_workqueue_init);
module_exit(example_workqueue_exit);

MODULE_DESCRIPTION("workqueue_struct example");
MODULE_LICENSE("GPL");
