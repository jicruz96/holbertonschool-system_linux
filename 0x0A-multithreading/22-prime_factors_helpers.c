#include "multithreading.h"


/**
 * exec_task - executes a task and saves the result
 * @task: task
 * Return: task result
 */
void *exec_task(task_t *task)
{
	void *result;

	result = task->entry(task->param);
	pthread_mutex_lock(&task->lock);
	task->result = result;
	pthread_mutex_unlock(&task->lock);
	return (result);
}

/**
 * get_task_status - gets a task status; thread-safe
 * @task: task
 * Return: task status
 */
task_status_t get_task_status(task_t *task)
{
	task_status_t status;

	pthread_mutex_lock(&task->lock);
	status = task->status;
	pthread_mutex_unlock(&task->lock);
	return (status);
}

/**
 * set_task_status - sets a task status; thread-safe
 * @task: task
 * @status: new status
 */
void set_task_status(task_t *task, task_status_t status)
{
	pthread_mutex_lock(&task->lock);
	task->status = status;
	pthread_mutex_unlock(&task->lock);
}
