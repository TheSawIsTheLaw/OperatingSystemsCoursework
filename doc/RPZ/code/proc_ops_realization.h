static int yaOpen(struct inode *spInode, struct file *spFile);

static ssize_t yaRead(struct file *filep, char __user *buf, size_t count, loff_t *offp);

static ssize_t yaWrite(struct file *file, const char __user *buf, size_t len, loff_t *offp);

static int yaRelease(struct inode *spInode, struct file *spFile);

static struct proc_ops fops = {proc_read : yaRead, proc_write : yaWrite, proc_open : yaOpen, proc_release : yaRelease};
