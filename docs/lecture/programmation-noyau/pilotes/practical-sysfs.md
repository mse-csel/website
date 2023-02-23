---
title: "Aspects pratiques : sysfs"
---

## exemple : méthodes d'accès aux attributs du périphérique

```c
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
```

## exemple : définition des structures et installation

```c
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
```
