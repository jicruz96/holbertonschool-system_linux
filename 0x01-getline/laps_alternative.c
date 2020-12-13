#include "laps.h"

/**
 * race_state - keeps track of laps made by several h in a race
 * @id: array of ints representing the id for each car in the race
 * @size: size of id array
 **/
void race_state(int *id, size_t size)
{
	static car_t *h;
	car_t *tmp, *new, *prev, **prev_to_new;
	unsigned int i;

	for (tmp = h; size == 0 && h; tmp = h->next, free(h), h = tmp)
		;
	for (i = 0; i < size; i++)
	{
		for (prev = NULL, tmp = h; tmp && id[i] > tmp->id; tmp = tmp->next)
			prev = tmp;
		if (tmp && id[i] == tmp->id)
		{
			tmp->laps += 1;
			continue;
		}
		printf("Car %d joined the race\n", id[i]);
		new = malloc(sizeof(car_t)), new->id = id[i], new->laps = 0;
		new->next = tmp, prev_to_new = prev ? &(prev->next) : &h;
		*prev_to_new = new;
	}
	if (size)
		for (printf("Race state:\n"), tmp = h; size && tmp; tmp = tmp->next)
			printf("%sCar %d [%d laps]\n", tmp->id, tmp->laps);
}
