#ifndef _INVERTEDINDEX_GUARD
#define _INVERTEDINDEX_GUARD

struct ListNode {
	char* urlName; 
    int searchTerms;
	double weightedPageRank;
	struct ListNode *next;
}; typedef struct ListNode *List;

struct ListRep {
	List first;
}; typedef struct ListRep *Rep;

Rep orderedList (Rep rep);
Rep newRep (void);
List newList (char *str, double weightedPageRank);
Rep insertList (char *str, double weightedPageRank, Rep rep);
#endif