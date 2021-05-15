#include "list.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>

List newList (char *str, double weightedPageRank) {
	List new = malloc (sizeof *new);
	int count = 0;
	int countLength = strlen(str);
	char *concreteWord1 = malloc((countLength+1)*sizeof(char));
	while (count < countLength) {
		concreteWord1[count] = str[count];
		count++;
	}
	concreteWord1[count] = '\0';
	new->urlName = concreteWord1;
	new->searchTerms = 0;
	new->weightedPageRank = weightedPageRank;
    new->next = NULL;
	return new;
}

Rep newRep (void) {
	Rep new = malloc (sizeof *new);
	new->first = NULL;
	return new;
}


Rep insertList (char *str, double weightedPageRank, Rep rep) {
	List curr = newList(str, weightedPageRank);
	if (rep->first == NULL) {	//if 1st item to be inserted
		rep->first = curr;
	} else {	//insert new node at the end of rep
		List curr1 = rep->first;
		while (curr1 != NULL) {
			if (strcmp(curr1->urlName, curr->urlName) == 0) {	//if node already exists
				curr1->searchTerms++;
				return rep;
			}
			curr1 = curr1->next;
		}
		curr1 = curr;
	}
	return rep;
}

//sorts existing unsorted list in descending order based on
//search terms and then weighted page rank
Rep orderedList (Rep rep1) {
    //create a new sorted list
    //set rep->first
    List finalList = newList(rep1->first->urlName, rep1->first->weightedPageRank);
	finalList->searchTerms = rep1->first->searchTerms;
    Rep rep = newRep();
    rep->first = finalList;
    //move to second item in original list
    List mainCurr = rep->first->next;	//iterates through the original rep
    while (mainCurr != NULL) {  //inserting each node from the original list
        finalList = rep->first;	//iterates through the newlist
        while (finalList != NULL) { //comparing to each node in the new list
            List new = newList(mainCurr->urlName, mainCurr->weightedPageRank);   //create new node for new list
			new->searchTerms = mainCurr->searchTerms;
			if (mainCurr->searchTerms == finalList->searchTerms) {	//sort based on weightedPageRank
				if (mainCurr->weightedPageRank > rep->first->weightedPageRank) {	//if wPR greater than the first element
					new->next = rep->first;
					rep->first = new;
					break;
				}
				if (finalList->next == NULL) {  
					finalList->next = new;
					break;
				}
				//if inbetween two numbers
				if ((mainCurr->weightedPageRank < finalList->weightedPageRank) && (mainCurr->weightedPageRank > finalList->next->weightedPageRank)) {
					List temp = finalList->next;
					finalList->next = new;
					new->next = temp;
					break;
				}
			} else {
				if (mainCurr->searchTerms > rep->first->searchTerms) {    //if greater than the first element
					new->next = rep->first;
					rep->first = new;
					break;
				}
				if (finalList->next == NULL) {  
					finalList->next = new;
					break;
				}
				//if inbetween two numbers
				if ((mainCurr->searchTerms < finalList->searchTerms) && (mainCurr->searchTerms > finalList->next->searchTerms)) {
					List temp = finalList->next;
					finalList->next = new;
					new->next = temp;
					break;
				}
			}
            finalList = finalList->next;
        }
        mainCurr = mainCurr->next;  
    }
	return rep;
}
