#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_var = PTHREAD_COND_INITIALIZER;

int count = 0;
int sum=0;
void *functionCount1();
void *functionCount2();

#define COUNT_DONE 10
#define COUNT_HALT1 5
#define COUNT_HALT2 10

void main() {
pthread_t thread1, thread2;
pthread_create( &thread1, NULL, &functionCount1, NULL);
pthread_create( &thread2, NULL, &functionCount2, NULL);
pthread_join( thread1, NULL);
pthread_join( thread2, NULL);
printf("Final count: %d\n",count);
}

void *functionCount1() {
for(;;) {
// Lock mutex and then wait for signal to relase mutex
pthread_mutex_lock( &count_mutex );
// Wait while functionCount2() operates on count
// mutex unlocked if condition varialbe in functionCount2() signaled.
pthread_cond_wait( &condition_var, &count_mutex );
count++;
	sum = sum+count;
printf("Counter value functionCount1: %d\n",count);
printf("summation value : %d \n",sum);
pthread_mutex_unlock( &count_mutex );
if(count >= COUNT_DONE) return(NULL);
}
}

void *functionCount2() {
for(;;) {
pthread_mutex_lock( &count_mutex );
if( count < COUNT_HALT1 ) 
{
// Condition of if statement has been met. 
// Signal to free waiting thread by freeing the mutex.
// Note: functionCount1() is now permitted to modify "count".
pthread_cond_signal( &condition_var );
} else {
count++;
	sum = sum+count;
printf("Counter value functionCount2: %d\n",count);
printf("summation value : %d \n",sum);
}
pthread_mutex_unlock(&count_mutex);
if(count >= COUNT_DONE) return(NULL);

}
}