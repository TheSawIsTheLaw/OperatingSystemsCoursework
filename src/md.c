#include <linux/delay.h>
#include <linux/init.h>
#include <linux/init_task.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

#include <linux/kthread.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yakuba D.");

#define TIMES 5
#define DELAY_MS 10 * 1000

static struct proc_dir_entry *procFile;

#define LOG_SIZE 262144
#define TEMP_STRING_SIZE 512

static char log[LOG_SIZE] = { 0 };

#define PROC_FS_NAME "yakubaProcessAnalyzer"

#define PREFIX "~~[TASK INFO]~~:"

static int printTasks(void *arg)
{
    struct task_struct *task;
    size_t currentPrint = 1;

    char currentString[TEMP_STRING_SIZE];

    int sumLen = 0;

    printk("")

    while (currentPrint <= TIMES)
    {
        task = &init_task;

        memset(currentString, 0, TEMP_STRING_SIZE);
        snprintf(currentString, TEMP_STRING_SIZE, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~: %lu TIME\n", currentPrint);

        sumLen = strlen(currentString) + strlen(log);
        if (sumLen >= LOG_SIZE)
        {
            printk(KERN_ERR "%s not enough space in log (%d needed but %d accessible)\n", PREFIX, sumLen, LOG_SIZE);

            return -ENOMEM;
        }

        strcat(log, currentString);

        for_each_process(task)
        {
            if (rt_task(task))
            {
                memset(currentString, 0, TEMP_STRING_SIZE);
                snprintf(currentString, TEMP_STRING_SIZE,
                         "procID: %-5d, name: %15s\nprio: %3d, static_prio: %3d, normal_prio (with "
                         "scheduler policy): %3d, realtime_prio: %3d\n"
                         "delay: %10lld\nutime: %10lld (ticks), stime: %15lld (ticks)\n"
                         "Sched_rt_entity: timeout: %ld, watchdog_stamp: %ld, time_slice: %d\n\n",
                         task->pid, task->comm, task->prio, task->static_prio, task->normal_prio, task->rt_priority,
                         task->sched_info.run_delay, task->utime, task->stime, task->rt.timeout,
                         task->rt.watchdog_stamp, task->rt.time_slice);

                if (strlen(currentString) + strlen(log) >= LOG_SIZE)
                {
                    printk(KERN_ERR "%s not enough space in log\n", PREFIX);

                    return -ENOMEM;
                }

                strcat(log, currentString);
            }
        }

        currentPrint++;
        mdelay(DELAY_MS);
    }

    return 0;
}

int yaOpen(struct inode *spInode, struct file *spFile)
{
    printk(KERN_INFO "%s open called\n", PREFIX);

    try_module_get(THIS_MODULE);

    return 0;
}

ssize_t yaRead(struct file *filep, char __user *buf, size_t count, loff_t *offp)
{
    ssize_t logLen = strlen(log);

    printk(KERN_INFO "%s read called\n", PREFIX);

    if (copy_to_user(buf, log, logLen))
    {
        printk(KERN_ERR "%s copy_to_user error\n", PREFIX);

        return -EFAULT;
    }

    memset(log, 0, LOG_SIZE);

    return logLen;
}

ssize_t yaWrite(struct file *file, const char __user *buf, size_t len, loff_t *offp)
{
    printk(KERN_INFO "%s write called\n", PREFIX);

    return 0;
}

int yaRelease(struct inode *spInode, struct file *spFile)
{
    printk(KERN_INFO "%s release called\n", PREFIX);

    module_put(THIS_MODULE);

    return 0;
}

static struct proc_ops fops = {proc_read : yaRead, proc_write : yaWrite, proc_open : yaOpen, proc_release : yaRelease};

static int __init md_init(void)
{
    if (!(procFile = proc_create(PROC_FS_NAME, 0666, NULL, &fops)))
    {
        printk(KERN_ERR "%s proc_create error\n", PREFIX);

        return -ENOMEM;
    }

    kthread_run(printTasks, NULL, "taskPrintThread");

    printk(KERN_INFO "%s module loaded\n", PREFIX);

        return 0;
}

static void __exit md_exit(void)
{
    remove_proc_entry(PROC_FS_NAME, NULL);

    printk(KERN_INFO "%s Module goes away... It's his final message.\n", PREFIX);
}

module_init(md_init);
module_exit(md_exit);
