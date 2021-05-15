#ifndef _INVERTEDINDEX_GUARD
#define _INVERTEDINDEX_GUARD

struct LinkedListNode {
	int iteration; 
    double *array;
	struct LinkedListNode *next;
    struct LinkedListNode *prev;
}; 
typedef struct LinkedListNode *LinkedList;

struct SortedLinkedListNode {
	char* urlName; 
    int outLinks;
    double weightedPageRank;
	struct SortedLinkedListNode *next;
    // struct SortedLinkedListNode *prev;
}; 
typedef struct SortedLinkedListNode *SortedLinkedList;

struct ListRep {
	SortedLinkedList first;  /**< first node in list */
}; typedef struct ListRep *SortedListRep;


LinkedList newLinkedList (double N, int iteration);
SortedLinkedList newSortedLinkedList (char *str, int outLinks, double weightedPageRank);
SortedListRep newSortedListRep (void);

#endif