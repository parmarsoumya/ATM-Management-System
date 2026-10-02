#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/err.h>

static dev_t atm_dev;
static struct cdev atm_cdev;
static struct class *atm_class;

static int atm_open(struct inode *inode, struct file *file)
{
    pr_info("ATM driver: device opened\n");
    return 0;
}

static ssize_t atm_read(struct file *file,
                        char __user *buffer,
                        size_t length,
                        loff_t *offset)
{
    pr_info("ATM driver: read requested\n");
    return 0;
}

static ssize_t atm_write(struct file *file,
                         const char __user *buffer,
                         size_t length,
                         loff_t *offset)
{
    pr_info("ATM driver: write requested\n");
    return length;
}

static int atm_release(struct inode *inode, struct file *file)
{
    pr_info("ATM driver: device closed\n");
    return 0;
}

static const struct file_operations atm_fops = {
    .owner = THIS_MODULE,
    .open = atm_open,
    .read = atm_read,
    .write = atm_write,
    .release = atm_release,
};

static int __init atm_driver_init(void)
{
    int ret;

    pr_info("ATM driver: module loading\n");

    ret = alloc_chrdev_region(&atm_dev, 0, 1, "atm_device");
    if (ret < 0) {
        pr_err("ATM driver: failed to allocate device number\n");
        return ret;
    }

    pr_info("ATM driver: major=%d minor=%d\n",
            MAJOR(atm_dev), MINOR(atm_dev));

    cdev_init(&atm_cdev, &atm_fops);
    atm_cdev.owner = THIS_MODULE;

    ret = cdev_add(&atm_cdev, atm_dev, 1);
    if (ret < 0) {
        pr_err("ATM driver: failed to add cdev\n");
        unregister_chrdev_region(atm_dev, 1);
        return ret;
    }

    atm_class = class_create("atm_class");
    if (IS_ERR(atm_class)) {
        pr_err("ATM driver: failed to create class\n");
        cdev_del(&atm_cdev);
        unregister_chrdev_region(atm_dev, 1);
        return PTR_ERR(atm_class);
    }

    if (IS_ERR(device_create(atm_class, NULL, atm_dev,
                             NULL, "atm_device"))) {
        pr_err("ATM driver: failed to create device\n");
        class_destroy(atm_class);
        cdev_del(&atm_cdev);
        unregister_chrdev_region(atm_dev, 1);
        return -ENOMEM;
    }

    pr_info("ATM driver: /dev/atm_device created\n");

    return 0;
}

static void __exit atm_driver_exit(void)
{
    device_destroy(atm_class, atm_dev);
    class_destroy(atm_class);
    cdev_del(&atm_cdev);
    unregister_chrdev_region(atm_dev, 1);

    pr_info("ATM driver: module unloaded\n");
}

module_init(atm_driver_init);
module_exit(atm_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ATM Management System");
MODULE_DESCRIPTION("ATM Character Device Driver");
