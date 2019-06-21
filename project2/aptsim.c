/*
 aptsim.c: Safe apartment inspection problem skeleton code
 (c) Mohammad Hasanzadeh Mofrad, 2019
 (e) moh18@pitt.edu
*/
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
};

static struct cs1550_sem *mutex;
static int *numTenents;
static bool *agentIsPresent;
int startTime;

int main(int argc, char** argv) {

    if(argc != 5) {
        printf("Usage: %s -m <number of tenants> -k <number of agents>\n", argv[0]);
        return(1);
    }
    // Read arguments
    struct options opt;
    opt.num_tenants = atoi(argv[2]);
    opt.num_agents = atoi(argv[4]);

    startTime = time(NULL);

    //Allocate a shared memory region to store one semaphore
    struct cs1550_sem *sem = mmap(NULL, sizeof(struct cs1550_sem), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
    memset(sem, sizeof(struct cs1550_sem), 0);
    //Initialize the semaphore to 0
    sem->value = 0;
    int i = 0;

    // Allocate an array for storing tenant and agent ids
    pid_t *pids = (void *) mmap(NULL, sizeof(pid_t) * 2, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
    memset(pids, sizeof(pid_t) * 2, 0);

    struct cs1550_sem *a_sem = mmap(NULL, sizeof(struct cs1550_sem), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
    memset(a_sem, sizeof(struct cs1550_sem), 0);
    //Initialize the semaphore to 0
    a_sem->value = 0;

    struct cs1550_sem *aa_sem = mmap(NULL, sizeof(struct cs1550_sem), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
    memset(aa_sem, sizeof(struct cs1550_sem), 0);
    //Initialize the semaphore to 0
    aa_sem->value = 1;

    struct cs1550_sem *t_sem = mmap(NULL, sizeof(struct cs1550_sem), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
    memset(t_sem, sizeof(struct cs1550_sem), 0);
    //Initialize the semaphore to 0
    t_sem->value = 0;

    struct cs1550_sem *ti_sem = mmap(NULL, sizeof(struct cs1550_sem), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
    memset(ti_sem, sizeof(struct cs1550_sem), 0);
    //Initialize the semaphore to 0
    ti_sem->value = 0;

    struct cs1550_sem *atl_sem = mmap(NULL, sizeof(struct cs1550_sem), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
    memset(atl_sem, sizeof(struct cs1550_sem), 0);
    //Initialize the semaphore to 0
    atl_sem->value = 0;

    mutex = mmap(NULL, sizeof(struct cs1550_sem), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
    memset(mutex, sizeof(struct cs1550_sem), 0);
    //Initialize the semaphore to 0
    mutex->value = 1;

    numTenents = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
    memset(numTenents, sizeof(int), 0);
    *numTenents = 0;

    agentIsPresent = mmap(NULL, sizeof(bool), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
    memset(agentIsPresent, sizeof(bool), 0);
    *agentIsPresent = false;

    //Create two tenant and agent processes
    int pid = fork(); // Create the first child process
    if (pid == 0) {
        down(sem);
        printf("Parent tenant: I am the first child process with pid=%d.\n", getpid());
        pids[0] = getpid();

        // Launch tenant processes
        for(i = 0; i < opt.num_tenants; i++) {
            if(pids[0] == getpid()) {
                pid = fork();
                if(pid == 0) {
                    tenantArrives();
                    *numTenents++;
                    up(t_sem);
                    //printf("Sems from tenent: a_sem:%d aa_sem:%d t_sem:%d\n", a_sem->value, aa_sem->value, t_sem->value);
                    down(a_sem);
                    down(mutex);
                    viewApt();
                    tenantLeaves();
                    up(mutex);
                    up(ti_sem);
                    exit(0);
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
            sleep(5);
            down(sem);
            printf("Parent agent: I am the second child process with pid=%d.\n", getpid());
            pids[1] = getpid();

            // Launch agent processes
            for(i = 0; i < opt.num_agents; i++) {
                if(pids[1] == getpid()) {
                    pid = fork();
                    if(pid == 0) {
                        agentArrives();
                        down(aa_sem);
                        down(mutex);
                        *agentIsPresent=true;
                        up(mutex);
                        down(t_sem);
                        openApt();

                        atl_sem->value=(-1)*a_sem->value;
                        do
                        {
                          up(a_sem);
                          down(t_sem);
                          down(atl_sem);
                        }while(atl_sem->value > 0);
                        printf("POST LOOP\n");
                        down(mutex);
                        *agentIsPresent=false;
                        up(mutex);
                        up(aa_sem);
                        agentLeaves();
                        exit(0);
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
            sleep(5);
            up(sem);
            sleep(1);
            up(sem);

            printf("I am the parent of 2 children and grandparent of %d grandchildren with pid=%d.\n", (opt.num_tenants + opt.num_agents), getpid());
            printf("Now,  My tenant child process pid is %d and my agent child process pid is %d\n", pids[0], pids[1]);
            wait(NULL);
            wait(NULL);
            printf("Then, My tenant child process pid is %d and my agent child process pid is %d\n", pids[0], pids[1]);
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
    *numTenents--;
}
