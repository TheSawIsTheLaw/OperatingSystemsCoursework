 static inline int rt_prio(int prio)
{
	if (unlikely(prio < MAX_RT_PRIO))
		return 1;
	return 0;
}
