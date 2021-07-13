#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HDDsimugrad.netid.h"


int main ()
{
    //createtherequests
    int requests[];
    requests = createRequests();
    for (int i = 0; i < NUM_REQUESTS; ++i) {
        addToQueue(requests[i]);
    }
    //servicetherequests
    int r;
    r = getNextRequest();
    while (r >= 0) {
        r = getNextRequest();
    }

}



//---------------------------------------------
// insert value at end

int addRequest(IORequestNode **listP, int trackNum) {
    DListNode *currNode, *prevNode, *newNode;

    newNode = (DListNode *) malloc(sizeof(DListNode));
    newNode->value = trackNum;
    newNode->next = *listP;
    newNode->prev = NULL;
    if (*listP != NULL)
        (*listP)->prev = newNode;
    *listP = newNode;
    return(0);
}

//=====================================================================

int printList(IORequestNode *list) {
    IORequestNode *node;
    if (list == NULL)
        printf("[empty]\n");
    else {
        printf("[ ");
        node = list;
        while (node != NULL) {
            printf("%d ", node->trackNum);
            node = node->next;
        }
        printf("]\n");
    }

    return(0);
}

//=====================================================================
// find and return the first instance of the matching value; if it's
// not found, then return -1; if it is found, return 0;

int find(DListNode *list, int value) {
    DListNode *currNode;
    if (list == NULL)
        return(-1);

    currNode = list;
    while (currNode != NULL) {
        if (currNode->value == value)
            return(0);
        else
            currNode = currNode->next;
    }

    return(-1);
}

//=====================================================================
// delete ALL instances of the matching value
// if at least one instance was found, return 0; else return 1

int serviceNextRequest(IORequestNode **listP, int *headPosition, int *headDirection, SchedulerType schedType,
                       int *displacement) {
    DListNode *currNode, *prevNode, *nextNode;
    int rtnval;

    if (*listP == NULL)
        return(1);

    rtnval = 1;
    prevNode = NULL;
    currNode = *listP;
    while (currNode != NULL) {
        nextNode = currNode->next;
        if (currNode->value == displacement) {
            // delete currNode
            rtnval = 0;
            if (prevNode != NULL) {
                prevNode->next = currNode->next;
                if (currNode->next != NULL)
                    currNode->next->prev = prevNode;
            } else {
                *listP = currNode->next;
            }
            free(currNode);
        } else {
            prevNode = currNode;
        }
        currNode = nextNode;
    }

    return(rtnval);
}

#define MAX_TEST_LENGTH 16
#define LINELEN 256


int getInputs(long *seed, SchedulerType *schedType) {
    char line[LINELEN];
    int i, seedOK, schedTypeOK, numvals;

    seedOK = 0;
    schedTypeOK = 0;

    while ( ! seedOK ) {
        printf("seed (integer value): ");
        fgets(line, LINELEN, stdin);
        line[strlen(line)-1] = '\0'; // chop off the newline char
        for (i=0; i<strlen(line); ++i) // convert to lower-case
            line[i] = tolower(line[i]);
        if (strlen(line) > 0) {
            numvals = sscanf(line, "%ld", seed);
            if (numvals != 1) {
                printf("seed must be an integer value\n");
            } else {
                seedOK = 1;
            }
        }
    }

    while ( ! schedTypeOK ) {
        printf("scheduler (FCFS, SCAN, or SSTF): ");
        fgets(line, LINELEN, stdin);
        line[strlen(line)-1] = '\0'; // chop off the newline char
        for (i=0; i<strlen(line); ++i) // convert to lower-case
            line[i] = tolower(line[i]);
        if (strlen(line) > 0) {
            if ( !strcmp(line, "fcfs") ) {
                schedTypeOK = 1;
                *schedType = SCHEDULER_FCFS;
            } else if ( !strcmp(line, "sstf") ) {
                schedTypeOK = 1;
                *schedType = SCHEDULER_SSTF;
            } else if ( !strcmp(line, "scan") ) {
                schedTypeOK = 1;
                *schedType = SCHEDULER_SCAN;
            } else {
                printf("scheduler must be 'FCFS' or 'SCAN' or 'SSTF'\n");
            }
        }
    }

    return(0);
}

int testRandomRequests(SchedulerType schedType, int numRequests, int maxRequest, long seed) {
    IORequestNode *requestQueue;
    int i, trackNum;
    double r, meandpr;
    int displacementTotal;
    int headPosition, headDirection, displacement;

    srand48(seed);

    requestQueue = NULL;
    displacementTotal = 0;

    for (i=0; i<numRequests; ++i) {
        r = drand48();
        trackNum = (int) maxRequest * r;
        addRequest(&requestQueue, trackNum);
    }

    printRequestQueue(requestQueue);

    headPosition = maxRequest / 2;
    headDirection = 1;

    printf("[");
    trackNum = serviceNextRequest(&requestQueue, &headPosition,
                                  &headDirection, schedType, &displacement);
    while (trackNum >= 0) {
        printf(" %d", trackNum);
        displacementTotal = displacementTotal + displacement;
//  printf("next request: %d; displacement = %d; pos = %d  dir = %d\n",
//         trackNum, displacement, headPosition, headDirection);
        trackNum = serviceNextRequest(&requestQueue, &headPosition,
                                      &headDirection, schedType, &displacement);
    }
    printf(" ]\n");

    meandpr = (double) displacementTotal / numRequests;
    printf("%d requests; mean displacement per request = %.2f\n",
           numRequests, meandpr);
}
