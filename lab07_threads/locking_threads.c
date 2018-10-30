#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define SIZE 1000000
#define THREADS 4

// Global variables
float arr[SIZE];
int g_index = 0;
int g_sum = 0;
pthread_mutex_t sum_mutex;

void *add(){

	do{
		pthread_mutex_lock(&sum_mutex);
		g_sum += arr[g_index];
		g_index++;
		pthread_mutex_unlock(&sum_mutex);
	} while(g_index < SIZE);
}

int main(){
	clock_t running_time;
	int l_sum_main = 0;
	srand(time(NULL));
	pthread_t thread_arr[THREADS]; // creating 4 threads.
	pthread_mutex_init(&sum_mutex, NULL);

	for (int i = 0; i < SIZE; i++){
		arr[i] = rand() % 1000;
		l_sum_main += arr[i];
	}

	running_time = clock();
	for (int i = 0; i < THREADS; i++)
		pthread_create(&thread_arr[i], NULL, add, NULL);

	for (int i = 0; i < THREADS; i++)
		pthread_join(thread_arr[i], NULL);

	running_time = clock() - running_time;

	double time_taken = ((double)running_time)/CLOCKS_PER_SEC; // in seconds
	printf("The Global Sum: %d\n", g_sum);
	printf("The Local Sum: %d\n", l_sum_main);

	if(l_sum_main == g_sum){
		printf("The result checks out!\n");
	}
	printf("Total Time: %f secs\n", time_taken);
	pthread_mutex_destroy(&sum_mutex);
	pthread_exit (NULL);
	return 0;
}