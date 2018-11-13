#include <pthread.h>
#include <stdio.h>
#include <errno.h>

#define FIRST_ODD_NUM 1
#define FIRST_EVEN_NUM 2
#define MAX 10

pthread_mutex_t mainMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t oddMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t evenMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t mainCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t oddCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t evenCond = PTHREAD_COND_INITIALIZER;

void *print_even_nums(void *param) {
	pthread_mutex_lock( &evenMutex );       // grab the even mutex

    pthread_mutex_lock( &mainMutex );       // signal main that the even thread
    pthread_cond_signal( &mainCond );       // is locked and ready for action
    pthread_mutex_unlock( &mainMutex );

	int even_num_to_print;
	for(even_num_to_print = FIRST_EVEN_NUM; even_num_to_print < MAX; even_num_to_print += 2){
		pthread_cond_wait( &evenCond, &evenMutex );
		printf("%d ", even_num_to_print);
		
		pthread_mutex_lock( &oddMutex );    // signal the odd thread
        pthread_cond_signal( &oddCond );
        pthread_mutex_unlock( &oddMutex );
	}

	pthread_mutex_unlock( &evenMutex );
}

void *print_odd_nums(void *param) {
	pthread_mutex_lock( &oddMutex );        // grab the odd mutex

    pthread_mutex_lock( &mainMutex );       // signal main that the odd thread
    pthread_cond_signal( &mainCond );       // is locked and ready for action
    pthread_mutex_unlock( &mainMutex );

	int odd_num_to_print;
	for(odd_num_to_print = FIRST_ODD_NUM; odd_num_to_print < MAX; odd_num_to_print += 2){
		pthread_cond_wait( &oddCond, &oddMutex );
		printf("%d ", odd_num_to_print);
		
		pthread_mutex_lock( &evenMutex );   // signal the even thread
        pthread_cond_signal( &evenCond );
        pthread_mutex_unlock( &evenMutex );
	}

	pthread_mutex_unlock( &oddMutex );
}

int main(int argc, char *argv[]) {
	pthread_t even_thread; pthread_t odd_thread;
	pthread_mutex_lock( &mainMutex );

	pthread_create(&even_thread, NULL, print_even_nums, NULL);
	pthread_cond_wait( &mainCond, &mainMutex );
	pthread_create(&odd_thread, NULL, print_odd_nums, NULL);
	pthread_cond_wait( &mainCond, &mainMutex );

	pthread_mutex_unlock( &mainMutex );     // startup has completed, release the main mutex
    pthread_mutex_lock( &oddMutex );        // signal the odd thread to get things rolling
    pthread_cond_signal( &oddCond );
    pthread_mutex_unlock( &oddMutex );

	pthread_join(even_thread, NULL); pthread_join(odd_thread, NULL);
}