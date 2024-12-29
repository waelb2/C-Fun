#include <semaphore.h>
#include <sys/types.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <unistd.h>
#include "bibliotheque.h"



// function to create or get a shared memory block
int get_shared_block(char *file_name, int size, int project_id ){
    key_t key;

    // generating the key 
    key = ftok(file_name, project_id);

    if(key == IPC_ERR_RESULT){
        return IPC_ERR_RESULT; 
    }

    int shmid = shmget(key, size , 0666| IPC_CREAT);
    if (shmid == IPC_ERR_RESULT){
        perror("shmget error");
        return IPC_ERR_RESULT;
    }
    return shmid;
}

// function to attach a shared memory block to adr procces
char * attach_memory_block(char * file_name, int size ,int project_id ){
    int shmid = get_shared_block(file_name, size,project_id );
    char *memory_ptr;
    
    if(shmid == IPC_ERR_RESULT){
        return NULL;
    }
    memory_ptr = shmat(shmid, NULL, 0);
    if(memory_ptr == (char*)IPC_ERR_RESULT){
        perror("shmat");
        return NULL;
    }
    return memory_ptr;
}

// function to detach the shared memory
bool detach_memory_block(char *block){
    return (shmdt(block) != IPC_ERR_RESULT);
}

// function to remove the shared memory
bool destroy_memory_block(int shmid){
    
    if(shmid == IPC_ERR_RESULT){
        return NULL;
    }
   return (shmctl(shmid, IPC_RMID, NULL) != IPC_ERR_RESULT);
}
int get_semaphore(char *file_name, int project_id, int size){
    int sem_id;
    key_t cle = ftok(file_name, project_id);
    sem_id= semget(cle, size, IPC_CREAT | 0666);

    if(cle == IPC_ERR_RESULT){
        return IPC_ERR_RESULT; 
    }
    if (sem_id == IPC_ERR_RESULT) {
        perror("Erreur : impossible de créer l'ensemble des sémaphores");
        return IPC_ERR_RESULT; 
    }
    return sem_id;
}
// funciton to wait for prcess
void P(int sem_id, int sem_num)
{
  struct sembuf operation = {sem_num, -1, 0};
  semop(sem_id, &operation, 1);
}

// funciton to release for prcess
void V(int sem_id, int sem_num)
{
  struct sembuf operation = {sem_num, 1, 0};
  semop(sem_id, &operation, 1);
}


