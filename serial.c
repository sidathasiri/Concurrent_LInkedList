#include <stdio.h>
#include <stdlib.h>
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

int insertOperations;   //insert operations
int memberOperations;   //member operations
int deleteOperations;   //delete operations

void display_list(struct Node** head_pp);
int insert(int val, struct Node** head_pp);
int delete(int val, struct Node** head_pp);
int member(int val, struct Node* head_p);

//add initial entries to linkedlist
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

//insert operation
void add_operation(struct Node** head_pp){
    for(int i=0; i<insertOperations; i++){
        int randVal = rand() % 65536;
        insert(randVal, head_pp);
    }
}

//member operation
void member_operation(struct Node* head_p){
    for(int i=0; i<memberOperations; i++){
        int randVal = rand() % 65536;
        member(randVal, head_p);
     //   printf("\nsearching %d found %d \n", randVal, result);
    }
}

//delete operation
void delete_operation(struct Node** head_pp){
    for(int i=0; i<deleteOperations; i++){
        int randVal = rand() % 65536;
        delete(randVal, head_pp);
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
int main(int argc, char *argv[]){

    int iterations = 100;
    double sampleTime[iterations];
    double totalTime = 0;
    double avgTime;
    double sumOfSquare=0;
    double stdDev;

    double start, end;

    struct Node *head_p = NULL;
    struct Node **head_pp = &head_p;

    if(argc != 6){
        printf("\nEnter only 5 paramerters\n");
        exit(0);
    } 
    
    //read command-line arguments 
    initialEntries = strtol(argv[1], NULL, 10); 
    operationsCount = strtol(argv[2], NULL, 10);
    memberFraction = strtod(argv[3],NULL);
    insertFraction = strtod(argv[4],NULL);
    deleteFraction = strtod(argv[5],NULL);

    memberOperations = memberFraction*operationsCount;
    insertOperations = insertFraction*operationsCount;
    deleteOperations = deleteFraction*operationsCount;

     for(int x=0; x<100; x++){
        //initial insert
        initial_insert(head_pp);

        //display_list(head_pp);
        GET_TIME(start);
        member_operation(head_p);   //member operation
        add_operation(head_pp);     //insert operation
        delete_operation(head_pp);  //delete operation
        GET_TIME(end);
        clear_list(head_pp);
        sampleTime[x] = (end-start);
        totalTime += (end-start);
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
    printf("Average time (ms): %f\n", avgTime*1000);
    printf("Standerd Deviation (ms): %f", stdDev);
}

//display the current linkedlist
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

//insert element functionality
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

//delete element functionality
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

//member element functionality
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


