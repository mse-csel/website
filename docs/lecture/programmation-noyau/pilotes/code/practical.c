static int skeleton_open(struct inode* i, struct file* f)
{
    pr_info("skeleton : open operation... major:%d, minor:%d\n",
            imajor(i),
            iminor(i));
    if ((f->f_mode & (FMODE_READ | FMODE_WRITE)) != 0) {
        pr_info("skeleton : opened for reading & writing...\n");
    } else if ((f->f_mode & FMODE_READ) != 0) {
        pr_info("skeleton : opened for reading...\n");
    } else if ((f->f_mode & FMODE_WRITE) != 0) {
        pr_info("skeleton : opened for writing...\n");
    }
    return 0;
}

static int skeleton_release(struct inode* i, struct file* f)
{
    pr_info("skeleton: release operation...\n");
    return 0;
}

#define BUFFER_SZ 10000
static char s_buffer[BUFFER_SZ];
static ssize_t skeleton_read(struct file* f,
                             char __user* buf,
                             size_t count,
                             loff_t* off)
{
    // compute remaining bytes to copy, update count and pointers
    ssize_t remaining = BUFFER_SZ - (ssize_t)(*off);
    char* ptr         = s_buffer + *off;
    if (count > remaining) count = remaining;
    *off += count;
    // copy required number of bytes
    if (copy_to_user(buf, ptr, count) != 0) count = -EFAULT;
    pr_info("skeleton: read operation... read=%ld\n", count);
    return count;
}

static ssize_t skeleton_write(struct file* f,
                              const char __user* buf,
                              size_t count,
                              loff_t* off)
{
    // compute remaining space in buffer and update pointers
    ssize_t remaining = BUFFER_SZ - (ssize_t)(*off);
    // check if still remaining space to store additional bytes
    if (count >= remaining) count = -EIO;
    // store additional bytes into internal buffer
    if (count > 0) {
        char* ptr = s_buffer + *off;
        *off += count;
        ptr[count] = 0;  // make sure string is null terminated
        if (copy_from_user(ptr, buf, count)) count = -EFAULT;
    }
    pr_info("skeleton: write operation... written=%ld\n", count);
    return count;
}

static dev_t skeleton_dev;
static struct cdev skeleton_cdev;
static int __init skeleton_init(void)
{
    int status = alloc_chrdev_region(&skeleton_dev, 0, 1, "mymodule");
    if (status == 0) {
        cdev_init(&skeleton_cdev, &skeleton_fops);
        skeleton_cdev.owner = THIS_MODULE;
        status              = cdev_add(&skeleton_cdev, skeleton_dev, 1);
    }
    pr_info("Linux module skeleton loaded\n");
    return 0;
}
static void __exit skeleton_exit(void)
{
    cdev_del(&skeleton_cdev);
    unregister_chrdev_region(skeleton_dev, 1);
    pr_info("Linux module skeleton unloaded\n");
}

int skeleton_drv_probe(struct platform_device* pdev)
{
    struct device_node* dt_node = pdev->dev.of_node;
    if (dt_node) {
        int ret              = 0;
        const char* prop_str = 0;
        ret = of_property_read_string(dt_node, "attribute", &prop_str);
        if (prop_str && ret == 0)
            pr_info("attribute=%s (ret=%d)\n", prop_str, ret);
    }
    // ....
    return 0;
}

int drv_probe(struct platform_device* pdev)
{
    struct device_node* dt_node = pdev->dev.of_node;
    if (dt_node) {
        const unsigned int* prop_reg = 0;
        struct device_node* child    = 0;
        for_each_available_child_of_node(dt_node, child)
        {
            pr_info("child found: name=%s, fullname=%s\n",
                    child->name,
                    child->full_name);
            prop_reg = of_get_property(child, "reg", NULL);
            if (prop_reg != 0) {
                unsigned long reg = of_read_ulong(prop_reg, 1);
                pr_info("reg:%lu\n", reg);
            }
        }
    }
    // ....
    return 0;
}

#include <linux/device.h>          /* needed for sysfs handling */
#include <linux/platform_device.h> /* needed for sysfs handling */
static char sysfs_buf[1000];
ssize_t sysfs_show_attr(struct device* dev,
                        struct device_attribute* attr,
                        char* buf)
{
    strcpy(buf, sysfs_buf);
    return strlen(buf);
}
ssize_t sysfs_store_attr(struct device* dev,
                         struct device_attribute* attr,
                         const char* buf,
                         size_t count)
{
    int len = sizeof(sysfs_buf) - 1;
    if (len > count) len = count;
    strncpy(sysfs_buf, buf, len);
    sysfs_buf[len] = 0;
    return len;
}
DEVICE_ATTR(attr, 0664, sysfs_show_attr, sysfs_store_attr);

static void sysfs_dev_release(struct device* dev) {}
static struct platform_device sysfs_device = {
    .name        = "mymodule",
    .id          = -1,
    .dev.release = sysfs_dev_release,
};
static int __init skeleton_init(void)
{
    int status = 0;
    if (status == 0) status = platform_device_register(&sysfs_device);
    if (status == 0)
        status = device_create_file(&sysfs_device.dev, &dev_attr_attr);
    return status;
}
static void __exit skeleton_exit(void)
{
    device_remove_file(&sysfs_device.dev, &dev_attr_attr);
    platform_device_unregister(&sysfs_device);
}

static struct file_operations fops_value = {
    .read  = skeleton_read_value,
    .write = skeleton_write_value,
};
static struct proc_dir_entry* procfs_dir = 0;
static int __init skeleton_init(void)
{
    int status = 0;
    procfs_dir = proc_mkdir("mymodule", NULL);
    proc_create("value", 0, procfs_dir, &fops_value);
    if (procfs_dir == 0) status = -EFAULT;
    return status;
}
static void __exit skeleton_exit(void)
{
    remove_proc_entry("value", procfs_dir);
    remove_proc_entry("mymodule", NULL);
}