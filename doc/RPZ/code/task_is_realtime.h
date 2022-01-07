 static inline bool task_is_realtime(struct task_struct *tsk)
{
	int policy = tsk->policy;

	if (policy == SCHED_FIFO || policy == SCHED_RR)
		return true;
	if (policy == SCHED_DEADLINE)
		return true;
	return false;
}
