/*
 sem.h: CS1550 semaphore helper library
 (c) Mohammad Hasanzadeh Mofrad, 2019
 (e) moh18@pitt.edu
*/

typedef struct cs1550_node{
  struct task_struct *process; //process id
  struct node *next;
} node;

#ifndef SEM_H_INCLUDED
#define SEM_H_INCLUDED
struct cs1550_sem {
   int value;
   //linked list. data is PSID?
   node *head;
   node *tail;
};
#endif
