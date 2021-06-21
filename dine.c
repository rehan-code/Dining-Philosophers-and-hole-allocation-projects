#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

/**
 * Student Name: Rehan Nagoor Mohideen
 * Student ID: 110592
 */

sem_t mutex;
sem_t chopstick[25];

void * philosopher(void * num) {
	int phil = ((int *)num)[0];
    int numPhil = ((int *)num)[1];
    int numMeals = ((int *)num)[2];

    srand(time(0));

    sem_wait(&mutex);
	printf("Philosopher %d thinking\n",phil+1);
    for (int i = 0; i < numMeals; i++) {//for each meal
        //Requesting for chopsticks based on odd and even to avoid deaddlock
        if (phil % 2 == 1) {
            sem_wait(&chopstick[(phil+1)%numPhil]);
            sem_wait(&chopstick[phil]);
        } else {
            sem_wait(&chopstick[phil]);
            sem_wait(&chopstick[(phil+1)%numPhil]);
        }

        printf("Philosopher %d eating\n",phil+1);
        int randno = (rand() % 3) + 1;//generates a random number between 1 and 3 for eating
        sleep(randno);
        printf("Philosopher %d thinking\n",phil+1);

        //put down chopsticks
        sem_post(&chopstick[(phil+1)%numPhil]);
        sem_post(&chopstick[phil]);
        randno = (rand() % 3) + 1;//generates a random number between 1 and 3 for thinking
        sleep(randno);
    }
    
	sem_post(&mutex);

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {//if incorrect no of arguments
        printf("Usage: ./dine [num. of philosophers] [num. of times each one eats]\n");
        exit(0);
    }

    int numPhil = atoi(argv[1]);
    int numMeals = atoi(argv[2]);

    if (numPhil < 3 || numPhil > 25 || numMeals < 1 || numMeals > 1000) {//if out of range
        printf("Error: no. of philosophers should be in range 3-25 and \n\tno. of times each philosopher eats should be in range 1-1000\n");
        exit(0);
    }
    
    
	int i,a[numPhil*3];
	pthread_t tid[numPhil];
	
	sem_init(&mutex,0,numPhil);
	
	for(i = 0; i < numPhil; i++) {
		sem_init(&chopstick[i],0,1);
    }    
		
	for(i = 0; i < numPhil; i++){
        //every 3 spots contain the id, no. philosophers and no. of meals so the philosophers can access them
		a[i*3] = i;
        a[i*3+1] = numPhil;
        a[i*3+2] = numMeals;
		pthread_create(&tid[i],NULL,philosopher,(void *)&a[i*3]);//give the address of the ID so it can relatively accesss the other data
	}
	for(i = 0; i < numPhil; i++) {//wait for the threads to complete.
		pthread_join(tid[i],NULL);
    }

    return 0;
}