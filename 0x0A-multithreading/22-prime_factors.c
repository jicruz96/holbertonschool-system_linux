#include "multithreading.h"
#include "22-prime_factors_helpers.c"
#include <stdlib.h>


__attribute__((constructor)) void tasks_mutex_init(void)
{
	pthread_mutex_init(&tasks_mutex, NULL);
}

__attribute__((destructor)) void tasks_mutex_destroy(void)
{
	pthread_mutex_destroy(&tasks_mutex);
}


/**
 * create_task - creates a new task structure and returns a pointer to it
 * @entry: pointer to the entry function of the task
 * @param: parameter to be passed to entry function
 * Return: pointer to the created task structure
 **/
task_t *create_task(task_entry_t entry, void *param)
{
	task_t *task = malloc(sizeof(task_t));
	static unsigned int id;

	if (task)
	{	task->entry = entry;
		task->param = param;
		task->lock = tasks_mutex;
		task->status = PENDING;
		task->result = NULL;
		task->id = id++;
	}

	return (task);
}

/**
 * destroy_task - destroys a task
 * @task: task to destroy
 **/
void destroy_task(task_t *task)
{
	if (task)
	{
		list_destroy(task->result, free);
		free(task->result);
		free(task);
	}
}

/**
 * exec_tasks - executes a list of tasks
 * @tasks: NULL-terminated list of tasks
 * Return: ???
 **/
void *exec_tasks(list_t const *tasks)
{
	int tasks_pending = 1, task_id;
	node_t *node;

	if (tasks == NULL)
		pthread_exit(NULL);

	while (tasks_pending)
		for (tasks_pending = 0, node = tasks->head; node; node = node->next)
			if (get_task_status(node->content) == PENDING)
			{
				tasks_pending = 1;
				task_id = ((task_t *)node->content)->id;
				set_task_status(node->content, STARTED);
				tprintf("[%02d] Started\n", task_id);
				if (exec_task(node->content))
				{
					set_task_status(node->content, SUCCESS);
					tprintf("[%02d] Success\n", task_id);
				}
				else
				{
					set_task_status(node->content, FAILURE);
					tprintf("[%02d] Failure\n", task_id);
				}
			}

	return (NULL);
}
