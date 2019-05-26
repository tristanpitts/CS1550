/*
 sem.h: CS1550 semaphore helper library
 (c) Mohammad Hasanzadeh Mofrad, 2019
 (e) moh18@pitt.edu
*/

typedef struct node{
  int data; //process id
  struct node *next;
} node;

#ifndef SEM_H_INCLUDED
#define SEM_H_INCLUDED
struct cs1550_sem {
   int value;
   int size; //number of processes in queue
   //linked list. data is PSID?
   struct node *head;
   struct node *tail;
};
#endif
