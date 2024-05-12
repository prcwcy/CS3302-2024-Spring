#include <linux/module.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/list.h>
#include "umem.h"

MODULE_LICENSE("GPL");

// TODO: data structure definition

static int umem_open(struct inode *inode, struct file *filp)
{
    // TODO
    pr_info("todo\n");
    return 0;
}

static int umem_release(struct inode *inode, struct file *filp)
{
    // TODO
    pr_info("todo\n");
    return 0;
}

static long umem_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    long err;
    struct umem_info kern_umem_info;

    pr_info("umem_ioctl cmd: %u\n", cmd);

    switch (cmd)
    {
    case UMEM_IOC_MALLOC:
        err = get_user(kern_umem_info.umem_size, &((struct umem_info __user *)arg)->umem_size);
        if (err)
        {
            pr_info("umem_ioctl cmd malloc: failed to get umem_size\n");
            return err;
        }
        err = get_user(kern_umem_info.umem_pool, &((struct umem_info __user *)arg)->umem_pool);
        if (err)
        {
            pr_info("umem_ioctl cmd malloc: failed to get umem_pool\n");
            return err;
        }
        pr_info("umem_ioctl cmd malloc: %llu %llu\n", kern_umem_info.umem_size, kern_umem_info.umem_pool);
        if (kern_umem_info.umem_pool >= UMEM_NUM_POOL)
        {
            pr_info("umem_ioctl cmd malloc: invalid pool\n");
            return -EINVAL;
        }
        // TODO
        pr_info("todo\n");
        return -EINVAL;
        return 0;

    case UMEM_IOC_FREE:
        err = get_user(kern_umem_info.umem_addr, &((struct umem_info __user *)arg)->umem_addr);
        if (err)
        {
            pr_info("umem_ioctl cmd free: failed to get umem_addr\n");
            return err;
        }
        pr_info("umem_ioctl cmd free: %px\n", (void *)kern_umem_info.umem_addr);
        // TODO
        pr_info("todo\n");
        return -EINVAL;
        return 0;
    case UMEM_IOC_PAGE_FAULT:
        err = get_user(kern_umem_info.umem_addr, &((struct umem_info __user *)arg)->umem_addr);
        if (err)
        {
            pr_info("umem_ioctl cmd page_fault: failed to get umem_addr\n");
            return err;
        }
        pr_info("umem_ioctl cmd page_fault: %px\n", (void *)kern_umem_info.umem_addr);
        // TODO
        pr_info("todo\n");
        return -EINVAL;
        return 0;
    }

    return -EINVAL;
}

static dev_t devt;
static struct cdev cdev;
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = umem_open,
    .release = umem_release,
    .unlocked_ioctl = umem_ioctl,
    .compat_ioctl = umem_ioctl,
};
static struct class *cls;
static struct device *dev;

static void umem_pool_init(void)
{
    // TODO
    pr_info("todo\n");
}

static void umem_pool_destroy(void)
{
    // TODO
    pr_info("todo\n");
}

static int __init umem_init(void)
{
    int err;

    err = alloc_chrdev_region(&devt, 0, 1, UMEM_NAME);
    if (err)
    {
        goto err_alloc_chrdev_region;
    }

    cdev_init(&cdev, &fops);
    err = cdev_add(&cdev, devt, 1);
    if (err)
    {
        goto err_cdev_add;
    }

    cls = class_create(UMEM_NAME);
    if (IS_ERR(cls))
    {
        err = PTR_ERR(cls);
        goto err_class_create;
    }
    dev = device_create(cls, NULL, devt, NULL, UMEM_NAME);
    if (IS_ERR(dev))
    {
        err = PTR_ERR(dev);
        goto err_device_create;
    }
    pr_info("umem installed\n");
    umem_pool_init();
    return 0;
err_device_create:
    class_destroy(cls);
err_class_create:
err_cdev_add:
    cdev_del(&cdev);
    unregister_chrdev_region(devt, 1);
err_alloc_chrdev_region:
    return err;
}

static void __exit umem_exit(void)
{
    umem_pool_destroy();
    device_destroy(cls, devt);
    class_destroy(cls);
    cdev_del(&cdev);
    unregister_chrdev_region(devt, 1);
    pr_info("umem removed\n");
}

module_init(umem_init);
module_exit(umem_exit);