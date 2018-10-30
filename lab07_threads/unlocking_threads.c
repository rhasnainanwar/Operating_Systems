#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define SIZE 1000000
#define THREADS 4

// Global variables
float arr[SIZE];

void *add(int * tid){
	int sum = 0;
	int threadID = (int)tid;
	for(int i = threadID; i < SIZE; i += THREADS){
		sum += arr[i];
	}
	printf("The thread # %d has sum of %d\n", threadID, sum);
}

int main(){
	clock_t running_time;
	int l_sum_main = 0;
	srand(time(NULL));
	pthread_t thread_arr[THREADS]; // creating 4 threads.

	for (int i = 0; i < SIZE; i++){
		arr[i] = rand() % 1000;
		l_sum_main += arr[i];
	}

	running_time = clock();
	for (int i = 0; i < THREADS; i++)
		pthread_create(&thread_arr[i], NULL, add,  (int *)i);

	for (int i = 0; i < THREADS; i++)
		pthread_join(thread_arr[i], NULL);

	running_time = clock() - running_time;

	double time_taken = ((double)running_time)/CLOCKS_PER_SEC; // in seconds

	printf("The Local Sum: %d\n", l_sum_main);

	printf("Total Time: %f secs\n", time_taken);
	pthread_exit (NULL);
	return 0;
}