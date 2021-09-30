#include <linux/delay.h>
#include <linux/init.h>
#include <linux/init_task.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/kthread.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yakuba D.");

#define TIMES 15
#define DELAY_MS 10 * 1000

static int printTasks(void *arg)
{
    struct task_struct *task;
    size_t currentPrint = 1;
    while (currentPrint <= TIMES)
    {
        task = &init_task;
        printk(KERN_INFO "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~: %lu TIME", currentPrint);
        for_each_process(task)
        {
            printk(KERN_INFO "~~[TASK INFO]~~: procID: %5d, name: %15s, prio: %4d, static_prio: %d, normal_prio (with "
                             "scheduler policy): %d, realtime_prio: %d "
                             "delay: %30lld, utime: %30lld (ticks), stime: %15d (ticks)",
                   task->pid, task->comm, task->prio, task->static_prio, task->normal_prio, task->rt_priority,
                   task->sched_info.run_delay, task->utime, task->stime);
            printk(KERN_INFO "Sched_rt_entity: timeout: %ld, watchdog_stamp: %ld, time_slice: %ld", task->rt.timeout,
                   task->rt.watchdog_stamp, task->rt.time_slice);
        }

        currentPrint++;
        mdelay(DELAY_MS);
    }

    return 0;
}

static int __init md_init(void)
{
    kthread_run(printTasks, NULL, "taskPrintThread");

    return 0;
}

static void __exit md_exit(void)
{
    printk(KERN_INFO "Module goes away... It's his final message.\n");
}

module_init(md_init);
module_exit(md_exit);
