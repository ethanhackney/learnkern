/* hello world kernel module */

/* needed by all modules */
#include <linux/module.h>

/* needed for pr_info() */
#include <linux/printk.h>

static int __init hello_init(void)
{
        pr_info("Hello World!\n");
        return 0;
}

static void __exit hello_exit(void)
{
        pr_info("Goodbye cruel world!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
