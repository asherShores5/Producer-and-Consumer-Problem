//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Authors: Asher Shores & Philip Varkey
//Assignment: Assignment 1: Producer and Consumer
//Instructor: CST-315, Operating Systems Lecture & Lab
//Date: January 23, 2022,
//This is my own work
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

//provides the number of threads required in the system to create one producer and one consumer
#define T_NUM 2

//initializes the mutex used in the system
pthread_mutex_t muBuffer;

//sets the buffer to a size of 100
int buff[100];
int val = 0;

_Noreturn void* producer() {
    while (1) {
        // initializes a random value between 1 and 100 to be created
        int prod = rand() % 100;

        //Sets the producer to sleep for one second
        sleep(1);

        //acquires a block to produce the values in the buffer
        pthread_mutex_lock(&muBuffer);

        //checks if the buffer is not full
        if (val < 100){
        //adds the random value to the buffer
		buff[val] = prod;
        	val++;
	}
        //removes the block in the system
        pthread_mutex_unlock(&muBuffer);

        // displays to the user that the value is produced
        printf("made %d\n", prod);
        //Sets the consumer to sleep for one second
        sleep(1);
    }
}

_Noreturn void* consumer() {
    while (1) {
        // initializes the value to identify the buffer
        int con = -1;

        //acquires a block to consume the values in the buffer
        pthread_mutex_lock(&muBuffer);

        //checks if the buffer has at least one value in it
        if (val > 0){
            con = buff[val - 1];
        	//consumes the value
            val--;
	}
        //removes the block in the system
        pthread_mutex_unlock(&muBuffer);

        // displays to the user that the value is consumed
        printf("Got %d\n", con);

        //Sets the consumer to sleep for one second
        sleep(1);
    }
}

int main() {
    srand(time(NULL));
    //Creates the number of threads required to run the system
    pthread_t th[T_NUM];
    pthread_mutex_init(&muBuffer, NULL);

    int i;
    //iterates over the number of threads to ensure their functionality
    for (i = 0; i < T_NUM; i++) {
        if (i > 0) {
            //Checks if the system failed to create a thread
            if (pthread_create(&th[i], NULL, &producer, NULL) != 0) {
                perror("Failed to create thread");
            }
        } else {
            if (pthread_create(&th[i], NULL, &consumer, NULL) != 0) {
                perror("Failed to create thread");
            }
        }
    }

    for (i = 0; i < T_NUM; i++) {
        //Checks if the system failed to join the thread
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }

    //removes the mutex to prevent any errors and to close the program fully
    pthread_mutex_destroy(&muBuffer);
    return 0;
}