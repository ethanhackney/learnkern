/* demonstrates command line argument passing to a module */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/stat.h>

MODULE_LICENSE("GPL");

/* parameters for this module */
static short    myshort = 1;
static long     mylong = 9999;
static char     *mystr = "hi:)";
static int      myarr[2] = { 420, 420 };
static int      myint = 420;
static int      arr_argc = 0;

/* module_param(name, type, mode)
 *
 * the first param is the parameters name
 * the second param is its data type
 * the final argument is the permission bits
 */
module_param(myshort, short, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(myshort, "A short integer");

module_param(mylong, long, S_IRUSR);
MODULE_PARM_DESC(mylong, "A long integer");

module_param(mystr, charp, 0000);
MODULE_PARM_DESC(mystr, "A character string");

module_param(myint, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(myint, "An integer");

/* module_param_array(name, type, num, perm):
 *
 * the first param is the parameter's (in this case the array's) name
 * the second param is the data type of the elements of the array
 * the third param is a pointer to the variable that will store the argc
 * the fourth argument is the permission bits
 */
module_param_array(myarr, int, &arr_argc, 0000);
MODULE_PARM_DESC(myarr, "An array of integers");

int __init args_init(void)
{
        int i;

        pr_info("myshort is a short integer: %hd\n", myshort);
        pr_info("mylong is a long integer:   %ld\n", mylong);
        pr_info("mystr is a string:          %s\n", mystr);
        pr_info("myint is an integer:        %d\n", myint);

        for (i = 0; i < ARRAY_SIZE(myarr); i++)
                pr_info("myarr[%d] = %d\n", i, myarr[i]);

        pr_info("got %d arguments for myarr\n", arr_argc);
        return 0;
}

void __exit args_exit(void)
{
        pr_info("cya:)");
}

module_init(args_init);
module_exit(args_exit);
