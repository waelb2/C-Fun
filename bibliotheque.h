#ifndef BIBLIOTHEQUE_H
#define BIBLIOTHEQUE_H

#include <stdbool.h>

#define N 6
#define IPC_ERR_RESULT (-1)
#define FILE_NAME "main.c"
#define BLOCK_SIZE_SEGMENT_1 2*sizeof(int)
#define BLOCK_SIZE_SEGMENT_2 N*sizeof(info)

typedef struct info  {
    int pid ;
    int rang ;
    int val ;
}info; 

// Function to create or get a shared memory block
int get_shared_block(char *file_name, int size, int project_id);

// Function to attach a shared memory block to the address space of the process
char *attach_memory_block(char *file_name, int size, int project_id);

// Function to detach the shared memory block from the process
bool detach_memory_block(char *block);

// Function to remove the shared memory block
bool destroy_memory_block(int shmid);

int get_semaphore(char *file_name,int size, int project_id);
// Semaphore operation: P (wait/decrement)
void P(int sem_id, int sem_num);

// Semaphore operation: V (signal/increment)
void V(int sem_id, int sem_num);

#endif // BIBLIOTHEQUE_H
