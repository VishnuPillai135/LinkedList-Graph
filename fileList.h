#ifndef _INVERTEDINDEX_GUARD
#define _INVERTEDINDEX_GUARD

struct LinkedListNode {
	int iteration; 
    double *array;
	struct LinkedListNode *next;
    struct LinkedListNode *prev;
}; 
typedef struct LinkedListNode *LinkedList;

LinkedList newLinkedList (int N, int iteration);

#endif