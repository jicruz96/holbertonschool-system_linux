#include "header.h"

/**
 * sort_file_list_by_time - sorst a file_node_t linked list by time
 * @head: head of linked list
 * Return: pointer to head
 **/
file_node_t *sort_file_list_by_time(file_node_t *head)
{
	file_node_t *this, *that, *tmp;
	time_t a, b;
	char *c, *d;

	if (!head || !head->next)
		return (head);

	for (this = head, that = head->next; that; that = that->next)
	{
		a = this->info->st_mtime, b = that->info->st_mtime;
		c = this->name, d = that->name;
		if (a > b || (a == b && which_goes_first(c, d) == c))
		{
			if (this->next != that)
			{
				if (this->prev)
					this->prev->next = this->next;
				else
					head = this->next;
				this->next->prev = this->prev, this->next = that;
				this->prev = that->prev, that->prev->next = this;
				that->prev = this;
			}
			this = that;
		}
		else if ((!that->next && (a < b || which_goes_first(c, d) == d)))
		{
			if (this->prev)
				this->prev->next = this->next;
			else
				head = this->next;
			this->next->prev = this->prev, tmp = this->next, this->next = NULL;
			this->prev = that, that->next = this, this = tmp, that = this;
		}
	}

	return (confirm_sorted_by_time(head));
}

/**
 * confirm_sorted_by_time - helper function for sort_file_list_by_time
 * @head: head of list
 * Return: pointer to head of list
 **/
file_node_t *confirm_sorted_by_time(file_node_t *head)
{
	file_node_t *tmp;

	for (tmp = head; tmp->next; tmp = tmp->next)
	{
		if (tmp->info->st_mtime > tmp->next->info->st_mtime)
			continue;
		if (tmp->info->st_mtime < tmp->next->info->st_mtime)
			return (sort_file_list_by_time(head));
		if (which_goes_first(tmp->name, tmp->next->name) != tmp->name)
			return (sort_file_list_by_time(head));
	}

	return (head);
}

/**
 * sort_file_list_by_size - sorst a file_node_t linked list by size
 * @head: head of linked list
 * Return: pointer to head
 **/
file_node_t *sort_file_list_by_size(file_node_t *head)
{
	file_node_t *this, *that, *tmp;
	size_t a, b;
	char *c, *d;

	if (!head || !head->next)
		return (head);

	for (this = head, that = head->next; that; that = that->next)
	{
		a = this->info->st_size, b = that->info->st_size;
		c = this->name, d = that->name;
		if (a > b || (a == b && which_goes_first(c, d) == c))
		{
			if (this->next != that)
			{
				if (this->prev)
					this->prev->next = this->next;
				else
					head = this->next;
				this->next->prev = this->prev, this->next = that;
				this->prev = that->prev, that->prev->next = this;
				that->prev = this;
			}
			this = that;
		}
		else if ((!that->next && (a < b || which_goes_first(c, d) == d)))
		{
			if (this->prev)
				this->prev->next = this->next;
			else
				head = this->next;
			this->next->prev = this->prev, tmp = this->next, this->next = NULL;
			this->prev = that, that->next = this, this = tmp, that = this;
		}
	}

	return (confirm_sorted_by_size(head));
}

/**
 * confirm_sorted_by_size - helper function for sort_file_list_by_size
 * @head: head of list
 * Return: pointer to head of list
 **/
file_node_t *confirm_sorted_by_size(file_node_t *head)
{
	file_node_t *tmp;

	for (tmp = head; tmp->next; tmp = tmp->next)
	{
		if (tmp->info->st_size > tmp->next->info->st_size)
			continue;
		if (tmp->info->st_size < tmp->next->info->st_size)
			return (sort_file_list_by_size(head));
		if (which_goes_first(tmp->name, tmp->next->name) != tmp->name)
			return (sort_file_list_by_size(head));
	}

	return (head);
}
