#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <err.h>
#include <assert.h>
#include <stdlib.h>
#include <sysexits.h>
#include <math.h>
#define maxArrayLength 9999
#include "graph.h" 
#include "fileList.h"

void PageRankW(double d, double diffPR, int maxIterations);
char *normaliseURL(char *str);

//add .txt to end of the given url
char *normaliseURL(char *str) {
    int countLength = strlen(str);
    
    while (str[0] == ' ') { //delete all leading spaces
        str = &str[1];
        countLength--;
    } 

    while (str[countLength - 1] == ' ') {   //delete all trailing spaces
        str[countLength - 1] = '\0';
        countLength--;
    }

    char src[countLength];  //append old string with ".txt"
    char dest[] = ".txt\0";
    int count = 0;
    while (count < countLength) {
        src[count] = str[count];
        count++;
    }
    src[count] = '\0';
    strcat(src, dest);
    char *pointer = src;
    return pointer;
}

//find vertex row/column of given url
int findVertex (char *str) {
    FILE* collectionTXT = fopen("collection.txt","r");
    char fileBuffer[maxArrayLength]; 
    int count = 0;
    int count2 = 0;
    //copy str pointer into an array so that array value does not change
    char collectionFile[maxArrayLength];
    while (str[count2] != '\0') {
        collectionFile[count2] = str[count2];
        count2++;
    }
    collectionFile[count2] = '\0';
    while (fscanf(collectionTXT, "%s", fileBuffer) != EOF) { //iterate through collection.txt to find matching value
        char *pointer = fileBuffer;
        pointer = normaliseURL(pointer);
        //copy into array
        //copy pointer pointer into an array so that value does not change
        char urlFile[maxArrayLength];
        int count1 = 0;
        while (pointer[count1] != '\0') {
            urlFile[count1] = pointer[count1];
            count1++;
        }
        urlFile[count1] = '\0';
        if (strcmp(urlFile, collectionFile) == 0) {
            fclose(collectionTXT);
            return count;
        }
        count++;
    }
    fclose(collectionTXT);
    return count;
}

double findN (void) {  //find amount of URLs in collection.txt
    FILE *collectionTXT = fopen("collection.txt", "r");
    char fileBuffer[maxArrayLength];
    double count = 0;
    while (fscanf(collectionTXT, "%s", fileBuffer) != EOF) {
        count++;
    }
    fclose(collectionTXT);
    return count;
}

double outInEdges (Graph graph, int vertex) {
    //find amount of edges
    double numEdges = 0;
    int count = 0;
    while (count < graph->nV) {
        if (count != vertex) {  //avoiding self-loop
            if (graph->edges[vertex][count] == 1) {
                numEdges++;
            }
        }
        count++;
    }
    return numEdges;
}

double WOut (Graph graph, int v, int u) {
    //find Wout
    double numerator = outInEdges(graph, u);
    if (numerator == 0) {
        numerator = 0.5;
    }
    double sum = 0;
    int count = 0;
    while (count < graph->nV) {
        if (graph->edges[v][count] == 1) {
            sum = sum + outInEdges(graph, count);
            if (outInEdges(graph, count) == 0) {
                sum = sum + 0.5;
            }
        }
        count++;
    }
    double division = numerator/sum;
    return division;
}

double WIn (Graph graph, int v, int u) {    
    //find Win
    double numerator = outInEdges(graph, u);
    double sum = 0;
    int count = 0;
    while (count < graph->nV) {
        if (graph->edges[count][v] == 1) {
            sum = sum + outInEdges(graph, count);
        }
        count++;
    }
    double division = numerator/sum;
    return division;
}

double sumPRWinWout (Graph inGraph, Graph outGraph, int u, LinkedList List) {
    //sum function
    int count = 0;
    double sum = 0;
    while (count < inGraph->nV) {
        if (inGraph->edges[u][count] == 1) {
            if (u != count) {   //not parallel edge
                double PR = List->array[count];
                double WIn_ = WIn(inGraph, count, u);
                double WOut_ = WOut(outGraph, count, u);
                double multiplication = PR * WIn_ * WOut_;
                sum = sum + multiplication;
            }
        }
        count++;
    }
    return sum;
} 

LinkedList calculatePR (Graph inGraph, Graph outGraph, int u, LinkedList List, LinkedList newList, double d, double N) {
    //PR function
    double numerator = 1.0-d;
    double denominator = N;
    double division = numerator/denominator;
    double sumPR = sumPRWinWout(inGraph, outGraph, u, List);
    double sum = division + d * sumPR;

    newList->array[u] = sum;
    return newList;
}

double findDiff(LinkedList List, LinkedList newList, int N) {   //diff function
    int count = 0;
    double sum = 0;
    while (count < N) {
        double PR1 = newList->array[count];
        double PR2 = List->array[count];
        double diff = PR1 - PR2;
        if (diff < 0) {
            diff = diff * -1.0;
        }
        sum = sum + diff;
        count++;
    }
    return sum;
}


void PageRankW(double d, double diffPR, int maxIterations) {
    //count number of urls in collection
    FILE* collectionTXT = fopen("collection.txt","r");
    FILE* urlTXT;
    char fileBuffer[maxArrayLength]; 
    char urlBuffer[maxArrayLength];
    double N = findN();
    //create empty graph using Matrix Representation
    Graph outGraph = newGraph(N);
    Graph inGraph = newGraph(N);

    while (fscanf(collectionTXT, "%s", fileBuffer) != EOF) { // 1.) scan through collection.txt for each url
         //don't include the first two words
        char *pointer1 = fileBuffer;
        pointer1 = normaliseURL(fileBuffer);  
        char rowArray[maxArrayLength];
        char columnArray[maxArrayLength];
        int count = 0;
        //save pointer1 value into rowArray so that value does not change
        while (pointer1[count] != '\0') {
            rowArray[count] = pointer1[count];
            count++;
        }
        rowArray[count] = '\0';
        urlTXT = fopen(pointer1, "r");
        while (fscanf(urlTXT, "%s", urlBuffer) != EOF) {    //2.) scan url file
            char *pointer3 = urlBuffer;
            pointer3 = normaliseURL(pointer3);
            count = 0;
            //save pointer1 value into columnArray so that value does not change
            while (pointer3[count] != '\0') {
                columnArray[count] = pointer3[count];
                count++;
            }
            columnArray[count] = '\0';
            if (strcmp(columnArray, "#end.txt\0") == 0) {
                break;
            }
            if ((strcmp(columnArray, "#start.txt\0") != 0) && (strcmp(columnArray, "Section-1.txt\0") != 0)) { 
                //3.) fill in the graph
                int num1 = findVertex(rowArray);
                int num2 = findVertex(columnArray);
                if (num1 != num2) {
                    outGraph->edges[num1][num2] = 1;
                    inGraph->edges[num2][num1] = 1;
                    outGraph->nE++;
                    inGraph->nE++;
                }
            }
        }
        fclose(urlTXT);
    }
    fclose(collectionTXT);

    LinkedList mainLinkedList = newLinkedList(N, 0);
    int count = 0;
    double numerator = 1.0;

    //For each url pi in the collection, set PR to 1/N
    while (count < N) {
        mainLinkedList->array[count] = numerator/N;
        count++;
    }
    int iteration = 0;
    double diff = diffPR;
    int countURL = 0;
    LinkedList nextLinkedList = NULL;
    while ((iteration < maxIterations) && (diff >= diffPR)) {   //the main while loop
        countURL = 0;
        nextLinkedList = newLinkedList(N, iteration + 1);
        mainLinkedList->next = nextLinkedList;
        nextLinkedList->prev = mainLinkedList;
        while (countURL < N) {  //setting value for PR(p;t+1)
            nextLinkedList = calculatePR(inGraph, outGraph, countURL, 
            mainLinkedList, nextLinkedList, d, N);
            countURL++;
        }
        diff = findDiff(mainLinkedList, nextLinkedList, N);
        mainLinkedList = mainLinkedList->next;
        iteration++;
    }


    //sort list
    count = 0;
    FILE* collectionTXT1 = fopen("collection.txt","r");
    char fileBuffer1[maxArrayLength];
    SortedLinkedList curr;
    SortedListRep Rep = newSortedListRep();
    while (fscanf(collectionTXT1, "%s", fileBuffer1) != EOF) {
        if (count == 0) {
            int outLinks = outInEdges(outGraph, count); 
            curr = newSortedLinkedList(fileBuffer1, outLinks, nextLinkedList->array[count]);
            Rep->first = curr;
        } else {
            int outLinks = outInEdges(outGraph, count); 
            curr->next = newSortedLinkedList(fileBuffer1, outLinks, nextLinkedList->array[count]);
            curr = curr->next;
        }
        count++;
    }
    fclose(collectionTXT1);
    
    //sort list in descending order
     // 4.) sort unsorted linked list in descending tf-idf order
    //create a new sorted list
    //set rep->first
    SortedLinkedList finalList = newSortedLinkedList(Rep->first->urlName, Rep->first->outLinks, Rep->first->weightedPageRank);
    SortedListRep rep = newSortedListRep();
    rep->first = finalList;
    //move to second item in original list
    SortedLinkedList mainCurr = Rep->first->next;
    while (mainCurr != NULL) {  //inserting each node from the original list
        finalList = rep->first;
        while (finalList != NULL) { //comparing to each node in the new list
            SortedLinkedList new = newSortedLinkedList(mainCurr->urlName, mainCurr->outLinks, mainCurr->weightedPageRank);   //create new node for new list
            if (mainCurr->weightedPageRank > rep->first->weightedPageRank) {    //if greater than the first element
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
                SortedLinkedList temp = finalList->next;
                finalList->next = new;
                new->next = temp;
                break;
            }
            finalList = finalList->next;
        }
        mainCurr = mainCurr->next;
        
    }

    mainCurr = rep->first;
    FILE *fp = fopen("pagerankList.txt", "w+");
    int lineCount = 1;
    while (mainCurr != NULL) {
        if (lineCount == findN()) { //ensuring that there is no extra new line
            fprintf(fp, "%s, %d, %.7f", mainCurr->urlName, mainCurr->outLinks, mainCurr->weightedPageRank);
        } else {
            fprintf(fp, "%s, %d, %.7f\n", mainCurr->urlName, mainCurr->outLinks, mainCurr->weightedPageRank);
        }
        lineCount++;
        mainCurr = mainCurr->next;
    }
    fclose(fp);

    return;
}


int main (int argc, char *argv[]) {
    double d = atof(argv[1]);
    double diffPR  = atof(argv[2]);
    int maxIterations = atoi(argv[3]);
    PageRankW(d, diffPR, maxIterations);
}

    