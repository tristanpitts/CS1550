/*
 aptsim.c: Safe apartment inspection problem skeleton code
 (c) Mohammad Hasanzadeh Mofrad, 2019
 (e) moh18@pitt.edu
*/

//Tristan Pitts CS1550 Summer 2019 Project 2
#include <sys/mman.h>
#include <linux/unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sched.h>
#include "sem.h"


void tenantArrives();
void agentArrives();
void viewApt();
void openApt();
void tenantLeaves();
void agentLeaves();

//Add your struct cs1550_sem type declaration below
void down(struct cs1550_sem *sem) {
    syscall(__NR_cs1550_down, sem);
}

void up(struct cs1550_sem *sem) {
    syscall(__NR_cs1550_up, sem);
}

// Structure to store command line arguments
struct options {
	int num_tenants;
	int num_agents;
  int tenent_prob;
  int tenent_delay;
  int tenent_seed;
  int agent_prob;
  int agent_delay;
  int agent_seed;
};

int startTime;//global variable for program start time

int main(int argc, char** argv) {

    if(argc != 17) {
        printf("Usage: %s -m <number of tenants> -k <number of agents> -pt -dt -st -pa -da -sa\n", argv[0]);
        return(1);
    }
    // Read arguments
    struct options opt;
    opt.num_tenants = atoi(argv[2]);
    opt.num_agents = atoi(argv[4]);
    opt.tenent_prob = atoi(argv[6]);
    opt.tenent_delay = atoi(argv[8]);
    opt.tenent_seed = atoi(argv[10]);
    opt.agent_prob = atoi(argv[12]);
    opt.agent_delay = atoi(argv[14]);
    opt.agent_seed = atoi(argv[16]);

    startTime = time(NULL);

    //Allocate a shared memory region to store one semaphore
    struct cs1550_sem *sem = mmap(NULL, sizeof(struct cs1550_sem), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
    memset(sem, sizeof(struct cs1550_sem), 0);
    //Initialize the semaphore to 0
    sem->value = 0;
    int i = 0;

    struct cs1550_sem *a_sem = mmap(NULL, sizeof(struct cs1550_sem), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
    memset(a_sem, sizeof(struct cs1550_sem), 0);
    //semaphore for tenent waiting on agent to arrive
    a_sem->value = 0;

    struct cs1550_sem *t_sem = mmap(NULL, sizeof(struct cs1550_sem), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
    memset(t_sem, sizeof(struct cs1550_sem), 0);
    //semaphore for agent waiting on tenents to arrive
    t_sem->value = 0;

    struct cs1550_sem *aa_sem = mmap(NULL, sizeof(struct cs1550_sem), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
    memset(aa_sem, sizeof(struct cs1550_sem), 0);
    //semaphore for agent waiting on agent to leave
    aa_sem->value = 1;

    struct cs1550_sem *signalAgent = mmap(NULL, sizeof(struct cs1550_sem), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
    memset(signalAgent, sizeof(struct cs1550_sem), 0);
    //semaphore for agent waiting on tenents to view apt and leave
    signalAgent->value = 0;

    struct cs1550_sem *mutex = mmap(NULL, sizeof(struct cs1550_sem), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
    memset(mutex, sizeof(struct cs1550_sem), 0);
    //semaphore for accessing shared data
    mutex->value = 1;

    int *remaining = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
    memset(remaining, sizeof(int), 0);
    *remaining=opt.num_tenants;
    //agent variable for keeping track of how many tenents remain

    int *seen = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
    memset(seen, sizeof(int), 0);
    *seen=0;
    //shared tenent variable keeping track of how many tenents have seen the apt so far

    // Allocate an array for storing tenant and agent ids
    pid_t *pids = (void *) mmap(NULL, sizeof(pid_t) * 2, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
    memset(pids, sizeof(pid_t) * 2, 0);

    //Create two tenant and agent processes
    int pid = fork(); // Create the first child process
    if (pid == 0) {
        down(sem);
        //printf("Parent tenant: I am the first child process with pid=%d.\n", getpid());
        pids[0] = getpid();

        // Launch tenant processes
        for(i = 0; i < opt.num_tenants; i++) {
            if(pids[0] == getpid()) {
                pid = fork();
                if(pid == 0) {
                    tenantArrives();
                    up(t_sem); //notify agent a tenant has arrived/is waiting
                    down(a_sem);//wait on agent
                    if(10*opt.num_agents < opt.num_tenants)
                      if(opt.num_tenants - *seen <=10)
                      {
                        tenantLeaves();
                        exit(0);
                      }
                    viewApt();
                    tenantLeaves();
                    down(mutex);
                    *seen+=1;//increment shared agent variable
                    up(mutex);
                    //printf("seen: %d\n", *seen);
                    if(*seen == opt.num_tenants || ((*seen)%10==0 && seen!=0))
                    {
                      //if this is the last tenant or the 10th tenent for the agent signal the agent to leave
                      up(signalAgent);
                    }
                    exit(0);
                }
              if(opt.tenent_prob!=100)//if a tenant is not gauranteed to show up
              {
                srand(opt.tenent_seed);//set seed for random number based on tenant seed input
                if(100-rand()%100 >= opt.tenent_prob)
                  sleep(opt.tenent_delay);
              }
            }
        }

        if(pids[0] == getpid()) {
            for(i = 0; i < opt.num_tenants; i++) {
                wait(NULL);
            }
        }
    }
    else {
        pid = fork(); // Create a second child process
        if(pid == 0) {
            down(sem);
            //printf("Parent agent: I am the second child process with pid=%d.\n", getpid());
            pids[1] = getpid();

            // Launch agent processes
            for(i = 0; i < opt.num_agents; i++) {
                if(pids[1] == getpid()) {
                    pid = fork();
                    if(pid == 0) {
                        agentArrives();
                        down(aa_sem); //wait if agent is already inside
                        if(*remaining==0)//if there are more agents than neccessary then once an agent is able to enter no tenants will be left
                        {
                          agentLeaves();
                          up(aa_sem);
                          exit(0);
                        }
                        openApt();
                        down(t_sem); //wait for tenents
                        int j;
                        for(j=0 ; j<10 ; j++)
                        {
                          up(a_sem);//for each call to up one tenant views the apt
                          *remaining-=1;
                          if(*remaining==0)//if the total tenents is not a multiple of ten this case is necessary
                            break;
                        }
                        down(signalAgent);//wait on the last tenant to leave the apt
                        if(10*opt.num_agents < opt.num_tenants)
                          if(*remaining<=10)
                          {
                            up(a_sem);
                          }
                        agentLeaves();
                        up(aa_sem);//next agent is able to enter
                        exit(0);
                    }
                    if(opt.agent_prob!=100)
                    {
                      srand(opt.agent_seed);//set seed for random based on
                      if(100-rand()%100 >= opt.agent_prob)
                        sleep(opt.agent_delay);
                    }
                }
            }

            if(pids[0] == getpid()) {
                for(i = 0; i < opt.num_agents; i++) {
                    wait(NULL);
                }
            }
        }
        else
        {
            up(sem);
            up(sem);

            //printf("I am the parent of 2 children and grandparent of %d grandchildren with pid=%d.\n", (opt.num_tenants + opt.num_agents), getpid());
            //printf("Now,  My tenant child process pid is %d and my agent child process pid is %d\n", pids[0], pids[1]);
            wait(NULL);
            wait(NULL);
            //printf("Then, My tenant child process pid is %d and my agent child process pid is %d\n", pids[0], pids[1]);
        }
    }

  return 0;
}

void tenantArrives()
{
    printf("Tenant %d arrives at time %d\n", getpid(), time(NULL) - startTime);
}

void agentArrives()
{
    printf("Agent %d arrives at time %d\n", getpid(), time(NULL) - startTime);
}

void viewApt()
{
    printf("Tenant %d inspects the apartment at time %d\n", getpid(), time(NULL) - startTime);
    sleep(2);
}

void openApt()
{
    printf("Agent %d opens the apartment for inspection at time %d\n", getpid(), time(NULL) - startTime);
}

void agentLeaves()
{
    printf("Agent %d leaves the apartment at time %d\n", getpid(), time(NULL) - startTime);
}

void tenantLeaves()
{
    printf("Tenant %d leaves the apartment at time %d\n", getpid(), time(NULL) - startTime);
}
