# completely_fair_scheduler
Implementation of the multimap ADT using a LLRB along with Google-style unit-testing. Program that receives a file of unordered task descriptions and feeds them into the CFS scheduler strategy.

Overall the scheduling loop follows several distinct steps:

    1) If there are tasks that should be launched at the current tick value, add them to the timeline of tasks.
       - Initialize their virtual runtime to the current global min_vruntime. This is to ensure that they will start executing soon, because they are inheriting the same priority as the next schedulable task.
    2) Check if the currently running task should let the processing resource to the next task. If it does, it should be added back to the timeline.
    3) If the current task has stopped running, get the next schedulable task from the timeline.
       - The next current tasks should be the task with the smallest virtual runtime. Here you note that if runnable tasks are kept in a Red-Black tree-based map, and ordered by their virtual runtime value (i.e., the key), then finding the task that has the minimum virtual runtime should be easy.
       - After determining the new current task, the global min_vruntime should be set to the next task’s virtual runtime.
    4) The current task runs for one tick.
    5) The scheduler reports the scheduling status on the screen.
    6) If the current task has completed, it should be purged from the system.
    7) Finally, the tick value is incremented by one, and the loop can continue from the beginning.

The scheduling loop stops when all tasks have been completed.
