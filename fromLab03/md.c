#include <linux/init.h>
#include <linux/init_task.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yakuba D.");

#define TIMES 5

static int __init md_init(void)
{
    // replace xd
    struct task_struct *task;
    int delayMS = 5000;

    size_t currentPrint = 1;
    while (currentPrint <= TIMES)
    {
        task = &init_task;
        printk(KERN_INFO "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~: %lu TIME", currentPrint);
        for_each_process(task)
        {
            printk(KERN_INFO "~~[TASK INFO]~~: name: %s, priority: %d, delay: %lld, runs: %lld (ticks)", task->comm, task->prio,
                   task->sched_info.run_delay, task->utime);
        }

        currentPrint++;
        mdelay(delayMS);
    }

    return 0;
}

static void __exit md_exit(void)
{
    printk(KERN_INFO "Module goes away... It's his final message. If you won't replace the battery...\n");
}

module_init(md_init);
module_exit(md_exit);
