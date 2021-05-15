#include "fileList.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>


LinkedList newLinkedList (int N, int iteration) {
	LinkedList new = malloc (sizeof *new);
	new->iteration = 0;
    new->array = calloc ((size_t) N, sizeof (int *));
    new->next = NULL;
    new->prev = NULL;
	return new;
}