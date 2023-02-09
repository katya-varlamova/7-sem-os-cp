#include <linux/init.h>
#include <linux/init_task.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Varlamova Ekaterina");

#define PROC_FS_NAME "analyzer"

#define TEMP_STRING_SIZE 512
#define LOG_SIZE 256 * 1024
static char log[LOG_SIZE] = { 0 };

static struct proc_dir_entry *proc_file;

int was_read = 1;

void print_tasks(void)
{
    struct task_struct *task, *process, *t;
    t = &init_task;
    char temp[TEMP_STRING_SIZE];
    memset(temp, 0, TEMP_STRING_SIZE);
    snprintf(temp, TEMP_STRING_SIZE,
             "%-5s %15s %6s %4s %5s %5s %5s %3s %6s %15s %15s %16s %16s %16s %6s %15s %18s %11s\n",
             "PID","name", "state", "prio", "sprio",
             "nprio", "rprio", "cpu", "policy",
             "utime", "stime", "exec_start","sum_exec_runtime",
             "vruntime", "pcount", "run_delay", "last_arrival", "last_queued");
    strcat(log, temp);
    do {
        struct task_struct *process = t;
        struct task_struct *task = process;
        do {
            if (strcmp(task->comm, "stress_pi_test") == 0) {
                memset(temp, 0, TEMP_STRING_SIZE);
                snprintf(temp, TEMP_STRING_SIZE,
                         "%-5d %15s %6d %4d %5d %5d %5d %3d %6d %15lld %15lld %16lld %16lld %16lld %6lu %15llu %18llu %11llu\n",
                         task->pid, task->comm, task->__state,
                         task->prio, task->static_prio, task->normal_prio, task->rt_priority,
                         task->cpu, task->policy,
                         task->utime, task->stime,
                         task->se.exec_start, task->se.sum_exec_runtime, task->se.vruntime,
                         task->sched_info.pcount, task->sched_info.run_delay, task->sched_info.last_arrival,
                         task->sched_info.last_queued);

                if (strlen(temp) + strlen(log) < LOG_SIZE)
                    strcat(log, temp);
                else
                    printk(KERN_ERR
                "max log size was exceeded!\n");
            }
        } while_each_thread(process, task);
    } while((t = next_task(t)) != &init_task);
        

}
static int my_open(struct inode *spInode, struct file *spFile)
{
    printk(KERN_INFO "open called\n");
    return 0;
}
static ssize_t my_read(struct file *filep, char __user *buf, size_t count, loff_t *offp)
{
    was_read = !was_read;
    if (was_read)
        return 0;

    memset(log, 0, LOG_SIZE);
    print_tasks();
    ssize_t logLen = strlen(log);
    printk(KERN_INFO "read called\n");

    if (copy_to_user(buf, log, logLen))
    {
        printk(KERN_ERR "copy_to_user error\n");

        return -EFAULT;
    }

    return logLen;
}

static int my_release(struct inode *spInode, struct file *spFile)
{
    printk(KERN_INFO "release called\n");
    return 0;
}


static struct proc_ops ops = {
        proc_read : my_read,
        proc_open: my_open,
        proc_release: my_release
};

static int __init md_init(void)
{
    if (!(proc_file = proc_create(PROC_FS_NAME, 0666, NULL, &ops)))
    {
        printk(KERN_ERR "proc_create error\n");
        return -EFAULT;
    }

    printk(KERN_INFO "module loaded\n");

    return 0;
}

static void __exit md_exit(void)
{
    remove_proc_entry(PROC_FS_NAME, NULL);
    printk(KERN_INFO "module exited\n");
}

module_init(md_init);
module_exit(md_exit);
