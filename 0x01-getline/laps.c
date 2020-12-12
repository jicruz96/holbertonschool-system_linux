#include "laps.h"

	/**
	 * NOTE: This file is irrelevant to the rest of the project. It's a program
	 * written for a school exercise to help us learn the basics about how to
	 * use static variables
	 **/

/**
 * race_state - keeps track of laps made by several cars in a race
 * @id: array of ints representing the id for each car in the race
 * @size: size of id array
 **/
void race_state(int *id, size_t size)
{
	static car_t *cars;
	car_t *tmp, *new, *prev;
	unsigned int i;


	/* if size is 0, free everything */
	if (size == 0)
	{
		free_car_list(&cars);
		return;
	}

	if (!id)
		return;

	/* Loop through ids. If id in list, increment laps. Else, add to list */
	for (i = 0; i < size; i++)
	{
		for (prev = NULL, tmp = cars; tmp && id[i] > tmp->id; tmp = tmp->next)
			prev = tmp;
		if (tmp && id[i] == tmp->id)
		{
			tmp->laps += 1;
		}
		else
		{
			printf("Car %d joined the race\n", id[i]);
			new = car_init(id[i], tmp);
			if (prev)
				prev->next = new;
			else
				cars = new;
		}
	}

	/* Print list */
	printf("Race state:\n");
	for (tmp = cars; tmp; tmp = tmp->next)
		printf("Car %d [%d laps]\n", tmp->id, tmp->laps);
}

/**
 * car_init - initialize a new car node
 * @id: id of new car
 * @next: next car node in the car list
 * Return: pointer to new car node
 **/
car_t *car_init(int id, car_t *next)
{
	car_t *new = malloc(sizeof(car_t));

	if (!new)
		return (NULL);
	new->id = id;
	new->laps = 0;
	new->next = next;
	return (new);
}

/**
 * free_car_list - frees all nodes in a car list
 * @cars: head of car list
 **/
void free_car_list(car_t **cars)
{
	car_t *prev, *tmp;

	if (!cars)
		return;

	prev = *cars;

	for (tmp = (*cars)->next; tmp; tmp = tmp->next)
	{
		free(prev);
		prev = tmp;
	}
	free(prev);
	*cars = NULL;
}
