#include <stdio.h>
#include <string.h>
#include "list.h"
#include <stdlib.h>
#define maxArray 999
#define BUFFER_SIZE 1024

int main( int argc, char* argv[] ){

    const char *delimiter_characters = "\n";
    const char *space = " ";
    const char *filename = "invertedIndex.txt";
    FILE *input_file = fopen( filename, "r" );
    char buffer[ BUFFER_SIZE ];
    char *firstToken;
    char *lastToken;
    int count = 0;
    Rep originalRep = newRep();
    if(input_file == NULL){
        fprintf( stderr, "Unable to open file %s\n", filename );
    }else{
        // Read each line into the buffer
        while(fgets(buffer, BUFFER_SIZE, input_file) != NULL){
            // Write the line to stdout
            //fputs( buffer, stdout );

            // Gets each token as a string and prints it
            firstToken = strtok(buffer, delimiter_characters);
            while( firstToken != NULL ){
                //tokenise it again
                lastToken = strtok(firstToken, space);
                count = 0;
                while( lastToken != NULL ){
                    char *concreteWord1;
                    if (count == 0) {
                        //copy url into a string so that value doesn't change
                        int countLength = strlen(lastToken);
                        int count1 = 0;
                        concreteWord1 = malloc((countLength+1)*sizeof(char));
                        while (count1 < countLength) {
                            concreteWord1[count1] = lastToken[count1];
                            count1++;
                        }
                        concreteWord1[count1] = '\0';
                    } else {
                        //check to see if url matches an argument
                        int countArg = 1;
                        while (argv[countArg] != NULL) {
                            if (strcmp(concreteWord1, argv[countArg]) == 0) {
                                FILE *pagerankList = fopen("pagerankList.txt", "r") ;
                                char buffer1[ BUFFER_SIZE ]; 
                                int count3 = 0;
                                double pageRankList;
                                while(fgets(buffer1, BUFFER_SIZE, pagerankList) != NULL) {  //find weightedPageRank
                                    if (count3 == 2) {
                                        pageRankList = atof(buffer1);
                                        break;
                                    }
                                    if (count3 == 1) {
                                        count3 = 2;
                                    }
                                    if (strcmp(lastToken, buffer1) == 0) {
                                        count3 = 1;
                                    }
                                }
                                //insert into the end of a list
                                originalRep = insertList(lastToken, pageRankList, originalRep);
                            }
                            countArg++;
                        }
                    }
                    lastToken = strtok(NULL, space);
                    count++;
                }
                firstToken = strtok(NULL, delimiter_characters);
            }
        }
        if( ferror(input_file) ){
            perror("The following error occurred");
        }
        fclose(input_file);

    }

    //sort existing unordered list
    Rep finalRep = newRep();
    finalRep = orderedList(originalRep);

    //count number of nodes in ordered list
    int countNodes = 0;
    List curr = finalRep->first;
    while (curr != NULL) {
        countNodes++;
    }

    //print out top 30 if greater than 30
    if (countNodes > 30) {
        countNodes = 0;
        curr = finalRep->first;
        while (countNodes <= 30) {
            printf("%s\n", curr->urlName);
            curr = curr->next;
            countNodes++;
        }
    } else {    //print all if less than 30
        countNodes = 0;
        curr = finalRep->first;
        while (curr != NULL) {
            printf("%s\n", curr->urlName);
            curr = curr->next;
        }
    }
    return 0;

}
