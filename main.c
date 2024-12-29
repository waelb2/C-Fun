#include "bibliotheque.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <unistd.h>

/**
 NOTE les variables globales ansi que les procedures qui vont etre re-utilisees dans les deux programmes (N, FILE_NAME, ...) sont declares dans bibliotheque.c
 * **/
int main(int argc, char const *argv[]) { 
    printf("Début du programme principal... \n\n");
    // Création des segments partagés pour cpt et v1  et le tableau acces_v1
    int v1_cpt_segment_id, acces_v1_segment_id;
    v1_cpt_segment_id = get_shared_block(FILE_NAME,BLOCK_SIZE_SEGMENT_1, 2);
    acces_v1_segment_id = get_shared_block(FILE_NAME,BLOCK_SIZE_SEGMENT_1,3);
    int *buffer_cpt_v1 ;
     
    // attachement du cpt et v1 
    buffer_cpt_v1 = (int *) attach_memory_block(FILE_NAME,0, 2);
    // Initialisation v1  
    buffer_cpt_v1[0] = 0 ;
    // Initialisation cpt
    buffer_cpt_v1[1] = 0;
    int sem_id ;   
    sem_id = get_semaphore(FILE_NAME,4, N+1);
    // Initialisation des semaphores a 0 et le dernier et le mutex a 1
    for(int i =0 ; i<N-1; i++){
        semctl(sem_id,i, SETVAL,0);
    }
    // le dernier semaphore
    semctl(sem_id, 5, SETVAL,1);
    // mutex
    semctl(sem_id, 6,SETVAL, 1);
    // Création des processus enfants
    for(int i= 0 ; i<N; i++){
        sleep(1);
        int p = fork();
        if(p == 0 ) {
            if(execl("./Pgme_processus", "./Pgme_processus",NULL)== -1 ){
                perror("Erreur : erreur dans l'execution du programme Pgme_processus");
                exit(1);
            }
        }
    }
    while(wait(NULL)!= -1);
    detach_memory_block((char*)buffer_cpt_v1);
    // Suppression des ressources IPC
    destroy_memory_block(v1_cpt_segment_id);
    destroy_memory_block(acces_v1_segment_id);

    // Suppression des sémaphores
    semctl(sem_id, 0, IPC_RMID);      
    printf("\n\nLibération des ressources IPC...\n");
    sleep(2);
    printf("\nProgramme terminé\n");

    return 0;
}
