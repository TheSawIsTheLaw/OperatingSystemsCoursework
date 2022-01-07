 static inline int rt_task(struct task_struct *p)
{
	return rt_prio(p->prio);
}
