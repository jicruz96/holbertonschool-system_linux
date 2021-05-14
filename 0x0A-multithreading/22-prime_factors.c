#include "multithreading.h"

/**
 * create_task - creates a new task structure and returns a pointer to it
 * @entry: pointer to the entry function of the task
 * @param: parameter to be passed to entry function
 * Return: pointer to the created task structure
 **/
task_t *create_task(task_entry_t entry, void *param)
{
	(void)entry;
	(void)param;
}

/**
 * destroy_task - destroys a task
 * @task: task to destroy
 **/
void destroy_task(task_t *task)
{
	(void)task;
}

/**
 * exec_tasks - executes a list of tasks
 * @tasks: NULL-terminated list of tasks
 * Return: ???
 **/
void *exec_tasks(list_t const *tasks)
{
	(void)tasks;
}

/*
For this task, you will have to write 3 functions (all in the same file):

Create task

Prototype: task_t *create_task(task_entry_t entry, void *param);, where
entry is a pointer to the entry function of the task,
param is the parameter that will later be passed to the entry function
This function must return a pointer to the created task structure
Destroy task

Prototype: void destroy_task(task_t *task);, where
task is a pointer to the task to destroy
Execute task list - Thread entry

Prototype: void *exec_tasks(list_t const *tasks);, where
tasks is a pointer to the list of tasks to be executed
This function serves as a thread entry
This function can safely return NULL as its return value will not be retrieved
This function must go through the list of tasks and execute them, but there’s a challenge:
Multiple thread will be going through the list of tasks, and a task must only be executed once
You must use tprintf to print when a task is started, and completed
*/
