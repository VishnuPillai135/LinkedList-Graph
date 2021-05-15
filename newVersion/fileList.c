#include "fileList.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>


LinkedList newLinkedList (double N, int iteration) {
	LinkedList new = malloc (sizeof *new);
	new->iteration = iteration;
    new->array = calloc ((size_t) N, sizeof (double *));
    new->next = NULL;
    new->prev = NULL;
	return new;
}

SortedLinkedList newSortedLinkedList (char *str, int outLinks, double weightedPageRank) {
	SortedLinkedList new = malloc (sizeof *new);
	int count = 0;
	int countLength = strlen(str);
	char *concreteWord1 = malloc((countLength+1)*sizeof(char));
	while (count < countLength) {
		concreteWord1[count] = str[count];
		count++;
	}
	concreteWord1[count] = '\0';
	new->urlName = concreteWord1;
	new->outLinks = outLinks;
    new->weightedPageRank = weightedPageRank;
    new->next = NULL;
    // new->prev = NULL;
	return new;
}

SortedListRep newSortedListRep (void) {
    SortedListRep new = malloc (sizeof *new);
    new->first = NULL;
    return new;
}
