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


int main() {
    int i, j ;
    info element;
    
    // declaration des variables pour acceder au segment partage cpt et v1
    int *buffer_cpt_v1;
    info *buffer_acces_v1;

    // attachement du cpt et v1 
    buffer_cpt_v1 = (int *) attach_memory_block(FILE_NAME,0, 2);
    buffer_acces_v1 = (info *) attach_memory_block(FILE_NAME,0, 3);

    // On obtient l'ensemble des semaphares
    int sem_id ;
    sem_id = get_semaphore(FILE_NAME,4, N+1);
    
    // mutex et incrementation du compteur
    P(sem_id,6);
    j= buffer_cpt_v1[1];
    buffer_cpt_v1[1]++;
    printf("Processus N: %d de PID: %d\n", j, getpid());
    V(sem_id,6);

    // Synchronisation et affectation des valeur pid, rang et valeur dans la table acces
    P(sem_id, j);
    buffer_cpt_v1[0]+=j;
    element.pid = getpid();
    element.rang= j;
    element.val= buffer_cpt_v1[0];
    buffer_acces_v1[j] = element;
    V(sem_id,(j-1 +N)% N);
    
    // Affichage des resultats
    if(j == 0){
        printf("\nLes resultats d'acces a la variable v1 : \n");
        sleep(1);
        for(int i= N-1;i >=0 ; i--  ){
            printf("\nAcces N: %d par le processus: %d, de rang = %d, v1 = %d", N-i+1,buffer_acces_v1[i].pid, buffer_acces_v1[i].rang, buffer_acces_v1[i].val );
        }
    }

    detach_memory_block((char*)buffer_acces_v1);
    detach_memory_block((char*)buffer_cpt_v1);
}
