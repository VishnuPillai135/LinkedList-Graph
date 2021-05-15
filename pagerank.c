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

LinkedList PageRankW(int d, int diffPR, int maxIterations);
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
    printf("src is %s\n", src);
    strcat(src, dest);
    printf("src is %s\n", src);
    char *pointer = src;
    printf("POINTER IS %s\n", pointer);
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
    double numEdges = 0;
    int count = 0;
    while (count < graph->nV) {
        if (count != vertex) {
            if (graph->edges[vertex][count] == 1) {
                numEdges++;
            }
        }
        count++;
    }
    return numEdges;
}

double WOutWIn (Graph graph, int v, int u) {
    double numerator = outInEdges(graph, u);
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

double sumPRWinWout (Graph inGraph, Graph outGraph, int u, LinkedList List) {
    int count = 0;
    double sum = 0;
    while (count < inGraph->nV) {
        if (inGraph->edges[u][count] == 1) {
            if (u != count) {   //not parallel edge
                double PR = List->array[count];
                double WIn = WOutWIn(inGraph, count, u);
                double WOut = WOutWIn(outGraph, count, u);
                double multiplication = PR * WIn * WOut;
                sum = sum + multiplication;
            }
        }
        count++;
    }
    return sum;
} 

LinkedList calculatePR (Graph inGraph, Graph outGraph, int u, LinkedList List, LinkedList newList, int d, int N) {
    double numerator = 1-d;
    double denominator = N;
    double division = numerator/denominator;
    double sumPR = sumPRWinWout(inGraph, outGraph, u, List);
    double sum = division + d + sumPR;

    newList->array[u] = sum;
    return newList;
}

double findDiff(LinkedList List, LinkedList newList, int N) {
    int count = 0;
    double sum = 0;
    while (count < N) {
        double PR1 = newList->array[count];
        double PR2 = List->array[count];
        double diff = PR1 - PR2;
        double absDiff = abs(diff);
        sum = sum + absDiff;
    }
    return sum;
}

//HAVE FOUND EACH OF THE TERMS, NOW PUT IT INTO THE MAIN FUNCTION :)

LinkedList PageRankW(int d, int diffPR, int maxIterations) {
    //count number of urls in collection
    FILE* collectionTXT = fopen("collection.txt","r");
    FILE* urlTXT;
    char fileBuffer[maxArrayLength]; 
    char urlBuffer[maxArrayLength];
    int N = findN();
    //create empty graph using Matrix Representation
    Graph outGraph = newGraph(N);
    Graph inGraph = newGraph(N);

    while (fscanf(collectionTXT, "%s", fileBuffer) != EOF) { // 1.) scan through collection.txt for each url
         //don't include the first two words
        char *pointer1 = fileBuffer;
        pointer1 = normaliseURL(fileBuffer);
        
        // char rowArray[maxArrayLength];
        // char columnArray[maxArrayLength];
        // int count = 0;
        //save pointer1 value into rowArray so that value does not change
        // while (pointer1[count] != '\0') {
        //     rowArray[count] = pointer1[count];
        //     count++;
        // }
        // rowArray[count] = '\0';
        char *rowArray = calloc(sizeof(char)*strlen(pointer1), 1);
        strcpy(rowArray, pointer1);
        printf("rowArray is %s ASFDF\n", pointer1);
        urlTXT = fopen(pointer1, "r");
        printf("rowArray is %s ASFDF\n", pointer1);
        printf("rowArray is %s\n", pointer1);
        while (fscanf(urlTXT, "%s", urlBuffer) != EOF) {    //2.) scan url file
            printf("URLBUFFER is %s for fileBuffer %s\n", urlBuffer, rowArray); 
            char *pointer3 = urlBuffer;
            pointer3 = normaliseURL(pointer3);
            // count = 0;
            //save pointer1 value into columnArray so that value does not change
            // while (pointer3[count] != '\0') {
            //     columnArray[count] = pointer3[count];
            //     count++;
            // }
            // columnArray[count] = '\0';
            char *columnArray = calloc(sizeof(char)*strlen(pointer3), 1);
            strcpy(columnArray, pointer3);
            // printf("columnAarray is %s\n", columnArray);
            if (strcmp(columnArray, "#end.txt") == 0) {  //cancel the loop
                break;
            } else if ((strcmp(columnArray, "#start.txt") != 0) && (strcmp(columnArray, "Section-1.txt") != 0)) {   //don't include the first two words
                //3.) fill in the graph
                int num1 = findVertex(rowArray);
                int num2 = findVertex(columnArray);
                outGraph->edges[num1][num2] = 1;
                inGraph->edges[num2][num1] = 1;
                outGraph->nE++;
                inGraph->nE++;
            }
            fclose(urlTXT);
            free(columnArray);
        }
        fclose(collectionTXT);
        free(rowArray);
    }
    //printing graph
    printGraph(outGraph, N);
    printf("\n\n");
    printGraph(inGraph, N);
    LinkedList mainLinkedList = newLinkedList(N, 0);
    int count = 0;
    double numerator = 1;

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
        nextLinkedList = newLinkedList(N, iteration + 1);
        mainLinkedList->next = nextLinkedList;
        nextLinkedList->prev = mainLinkedList;
        while (countURL < N) {
            nextLinkedList = calculatePR(inGraph, outGraph, countURL, mainLinkedList, nextLinkedList, d, N);
            countURL++;
        }
        diff = findDiff(mainLinkedList, nextLinkedList, N);
        mainLinkedList = mainLinkedList->next;
        iteration++;
    }

    return nextLinkedList;
}


int main (void) {
    double d;
    double diffPR;
    int maxIterations;
    scanf("%lf", &d);
    scanf("%lf", &diffPR);
    scanf("%d", &maxIterations);
    LinkedList mainLinkedList = PageRankW(d, diffPR, maxIterations);
    printf("sflkjslfjslkfs\n");
    int count = 0;
    while (count < findN()) {
        printf("%lf\n", mainLinkedList->array[count]);
        count++;
    }
    
    return 0;
}