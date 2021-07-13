#define NUM TRACKS 200

typedef struct DListNodeStruct {
    int value;
    struct DListNodeStruct *next;
    struct DListNodeStruct *prev;
} DListNode;


typedef enum {
    SCHEDULER_FCFS,
    SCHEDULER_SSTF,
    SCHEDULER_SCAN
} SchedulerType;

typedef struct IORequestNodeStruct {
    int trackNum;
    struct IORequestNodeStruct * prev;
    struct IORequestNodeStruct * next;
} IORequestNode;


//int insert(DListNode **list, int value);
int find(DListNode *list, int value);
//int delete(DListNode **list, int value);
//int printList(DListNode *list);

int addRequest(IORequestNode **listP, int trackNum);
int serviceNextRequest(IORequestNode **listP, int *headPosition, int *headDirection, SchedulerType schedType,
        int *displacement);
void printRequestQueue(IORequestNode *list);

int createRequests();
int getNextRequest();

