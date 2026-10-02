#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

static int __init atm_driver_init(void)
{
    pr_info("ATM driver: module loaded\n");
    return 0;
}

static void __exit atm_driver_exit(void)
{
    pr_info("ATM driver: module unloaded\n");
}

module_init(atm_driver_init);
module_exit(atm_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Educational ATM Linux driver prototype");
