#include "sched_idletask.c"
#include "sched_fair.c"
#include "sched_rt.c"
#ifdef CONFIG_SCHED_DEBUG
#include "sched_debug.c"
#endif

/*
 * __normal_prio - return the priority that is based on the static prio
 */
static inline int __normal_prio(struct task_struct *p) // _NORMAL_PRIO function, return static priority value
{
    return p->static_prio;
}

/*
 * Calculate the expected normal priority: i.e. priority
 * without taking RT-inheritance into account. Might be
 * boosted by interactivity modifiers. Changes upon fork,
 * setprio syscalls, and whenever the interactivity
 * estimator recalculates.
 */
static inline int normal_prio(struct task_struct *p) //  NORMAL_PRIO function
{
    int prio;

    if (task_has_rt_policy(p)) //  The task_has_rt_policy function, the determination process is a real-time process, if the real-time process, returns 1, otherwise returns 0
        prio = MAX_RT_PRIO-1 - p->rt_priority; //  The process is real-time process, and the PRIO value is related to the real-time priority value: PRIO = MAX_RT_PRIO -1 - P-> rt_priority
    else
        prio = __normal_prio(p); //  The process is a non-real-time process, then the PRIO value is a static priority value, that is, PRIO = P-> static_prio
    return prio;
}

/*
 * Calculate the current priority, i.e. the priority
 * taken into account by the scheduler. This value might
 * be boosted by RT tasks, or might be boosted by
 * interactivity modifiers. Will be RT if the task got
 * RT-boosted. If not then it returns p->normal_prio.
 */
static int effective_prio(struct task_struct *p) //  The Effective_Prio function, the effective priority of the calculation process, the PRIO value, this value is the priority value used by the final scheduler
{
    p->normal_prio = normal_prio(p); //  Calculate the value of Normal_PRIO
    /*
     * If we are RT tasks or we were boosted to RT priority,
     * keep the priority unchanged. Otherwise, update priority
     * to the normal priority:
     */
    if (!rt_prio(p->prio))
        return p->normal_prio; //  If the process is a non-real-time process, return normal_prio value, at this time Normal_Prio = Static_Prio
    return p->prio; //  Otherwise, the return value is constant, still PRIO value, at this time, PRIO = MAX_RT_PRIO -1 - P-> RT_Priority
}

/*************************************************/
void set_user_nice(struct task_struct *p, long nice)
{
    ...
    p->prio = effective_prio(p); //  In the function set_user_nice, call the Effective_Prio function to set the process's PRIO value.
    ...
}
