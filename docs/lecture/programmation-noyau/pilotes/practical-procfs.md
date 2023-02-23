---
title: "Aspects pratiques : procfs"
---

## exemple : installation des opérations

```c
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
```