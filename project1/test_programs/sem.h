/*
 sem.h: CS1550 semaphore helper library
 (c) Mohammad Hasanzadeh Mofrad, 2019
 (e) moh18@pitt.edu
*/

typedef struct cs1550_node{
  struct task_struct *process; //process id
  struct cs1550_node *next;
} node;

#ifndef SEM_H_INCLUDED
#define SEM_H_INCLUDED
struct cs1550_sem {
   int value;
   struct cs1550_node *head;
   struct cs1550_node *tail;
};
#endif
