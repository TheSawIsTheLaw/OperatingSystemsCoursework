 static inline int normal_prio(struct task_struct *p) //  NORMAL_PRIO function
{
    int prio;

    if (task_has_rt_policy(p)) //  The task_has_rt_policy function, the determination process is a real-time process, if the real-time process, returns 1, otherwise returns 0
        prio = MAX_RT_PRIO-1 - p->rt_priority; //  The process is real-time process, and the PRIO value is related to the real-time priority value: PRIO = MAX_RT_PRIO -1 - P-> rt_priority
    else
        prio = __normal_prio(p); //  The process is a non-real-time process, then the PRIO value is a static priority value, that is, PRIO = P-> static_prio
    return prio;
}