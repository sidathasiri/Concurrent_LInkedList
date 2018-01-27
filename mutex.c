#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include<math.h>
#include "timer.h"

struct Node{
    int value;
    struct Node* next;
};

int initialEntries;     //intial insertions to linkedlist
int operationsCount;    //totoal operations
float memberFraction;   //member operations fraction
float insertFraction;   //insert operation fraction
float deleteFraction;   //delete operation fraction
int threadCount;        //number of executing threads

struct Node *head_p = NULL;
struct Node **head_pp = &head_p;

pthread_mutex_t mutexLock;  //instance of mutex

int insertOperations;
int memberOperations;
int deleteOperations;

void display_list(struct Node** head_pp);
int insert(int val, struct Node** head_pp);
int delete(int val, struct Node** head_pp);
int member(int val, struct Node* head_p);

//method executed by threads
void* run_threads(void* rank){
    long id = (long) rank;
    int operationType;
    int localRun = operationsCount/threadCount;

    for(int i=0; i<localRun; i++){

        operationType = (id*localRun) + i;
        if(operationType < memberOperations){
            //member operation
            pthread_mutex_lock(&mutexLock);
            int randVal = rand() % 65536;
            member(randVal, head_p);
            pthread_mutex_unlock(&mutexLock);
        } else if(operationType < (memberOperations+insertOperations)){
            //insert operation
            pthread_mutex_lock(&mutexLock);
            int randVal = rand() % 65536;
            insert(randVal, head_pp);
            pthread_mutex_unlock(&mutexLock);
        } else {
            //delete operation
            pthread_mutex_lock(&mutexLock);
            int randVal = rand() % 65536;
            delete(randVal, head_pp);
            pthread_mutex_unlock(&mutexLock);
        }
        
    }
    return 0;
    
}

//insert intial elements to linkedlist
void initial_insert(struct Node** head_pp){
    int inserts = 0;
    while(inserts!=initialEntries){
        int randVal = rand()%65536;
        int result = member(randVal, *head_pp);
        if(result==0){
            insert(randVal, head_pp);
            inserts++;
        }
    }
}

void clear_list(struct Node  **root){
    struct Node *curr_p;
    struct Node *succ_p;

    if (*root == NULL) {
        return;
    }

    curr_p = *root;
    succ_p = curr_p->next;

    while (succ_p != NULL) {
        free(curr_p);
        curr_p = succ_p;
        succ_p = curr_p->next;
    }

    free(curr_p);
    *root = NULL;

}

//param 1 = #initial entries
//param 2 = #operations m
//param 3 = #member fraction
//param 4 = #insert fractoin
//param 5 = #delete fraction
//param 6 = #number of threads
int main(int argc, char *argv[]){

    int iterations = 100;
    double sampleTime[iterations];
    double totalTime = 0;
    double avgTime;
    double sumOfSquare=0;
    double stdDev;

    double start, end;
    long thread;
    pthread_t* thread_handles;

    if(argc != 7){
        printf("\nEnter only 6 paramerters\n");
        exit(0);
    } 
    
    //reading command-line arguments
    initialEntries = strtol(argv[1], NULL, 10); 
    operationsCount = strtol(argv[2], NULL, 10);
    memberFraction = strtod(argv[3],NULL);
    insertFraction = strtod(argv[4],NULL);
    deleteFraction = strtod(argv[5],NULL);
    threadCount = strtol(argv[6], NULL, 10);
    
    memberOperations = memberFraction*operationsCount;
    insertOperations = insertFraction*operationsCount;
    deleteOperations = deleteFraction*operationsCount;

   for(int i=0; i<iterations; i++){
        //initial insert
        initial_insert(head_pp);

        thread_handles = malloc(threadCount*sizeof(pthread_t));     //allocate memory for threads

        pthread_mutex_init(&mutexLock, NULL);   //initialize the mutex

        GET_TIME(start);        
        //execute threads
        for(thread = 0; thread < threadCount; thread++)
            pthread_create(&thread_handles[thread], NULL, run_threads, (void*) thread);
        for (thread = 0; thread < threadCount; thread++){
            pthread_join(thread_handles[thread], NULL);
        }
         
        GET_TIME(end);
        pthread_mutex_destroy(&mutexLock);      //destroy mutex
        sampleTime[i] = (end-start);
        totalTime += (end-start);
        clear_list(head_pp);
       
        free(thread_handles);   //release aquired memory
   }

    avgTime = totalTime/iterations;

     for(int i=0; i<iterations; i++){
         sumOfSquare += (sampleTime[i]-avgTime)*(sampleTime[i]-avgTime);
     }

     stdDev = sqrt(sumOfSquare/(iterations-1));

    printf("\nSummary......\n");
    printf("----------MEMBERING %d ITEMS COMPLETE--------------\n", memberOperations);
    printf("----------INSERTING %d ITEMS COMPLETE--------------\n", insertOperations);
    printf("----------DELETING %d ITEMS COMPLETE---------------\n", deleteOperations);
    printf("Total time (ms): %f\n", totalTime*1000);
    printf("Avg time (ms): %f\n", avgTime*1000);
    printf("Standerd Deviation time (ms): %f\n", stdDev);
}

//display elements of linkedlist 
void display_list(struct Node** head_pp){
    if(head_pp != NULL){
        struct Node* current = *head_pp;
        while(current != NULL){
            printf("%d, ", current->value);
            current = current->next;
        }
    } else {
        printf("Empty list");
    }
}

//insert element to linkedlist
int insert(int val, struct Node** head_pp){
    struct Node*current = *head_pp;
    struct Node* prev = NULL;
    struct Node* temp;

    while(current != NULL && current->value < val){
        prev = current;
        current = current->next;
    }
    if(current == NULL || current->value > val){
        temp = malloc(sizeof(struct Node));
        temp-> value = val;
        temp->next = current;
        if(prev == NULL){
            *head_pp = temp;
        } else {
            prev->next = temp;
        }
        return 1;
    } else {
        return 0;
    }
}

//delete element from linkedlist
int delete(int val, struct Node** head_pp){
    struct Node*current = *head_pp;
    struct Node* prev = NULL;

    while(current != NULL && current->value < val){
        prev = current;
        current = current->next;
    }

    if(current != NULL && current->value == val){
        if(prev == NULL){
            //deleting the head
            *head_pp = current->next;
            free(current);
        } else {
            prev->next = current->next;
            free(current);
        }
        return 1;
    } else{
        return 0;
    }
}

//member element in linkedlist
int member(int val, struct Node* head_p){
    struct Node* current = head_p;

    while(current != NULL && current->value < val){
        current = current->next;
    }

    if(current == NULL || current->value > val){
        return 0;
    } else {
        return 1;
    }
}


