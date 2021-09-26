#include <linux/init.h>
#include <linux/init_task.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yakuba D.");

static int __init md_init(void)
{
    printk(KERN_INFO "Module init:\n");

    struct task_struct *task;
    task = &init_task;
    for_each_process(task)
    {
        printk(KERN_INFO "~~[TASK INFO]~~ name: %s, priority: %d", task->comm, task->prio);
    }

    return 0;
}

static void __exit md_exit(void)
{
    printk(KERN_INFO "Module goes away... It's his final message. If you won't replace the battery...\n");
}

module_init(md_init);
module_exit(md_exit);
