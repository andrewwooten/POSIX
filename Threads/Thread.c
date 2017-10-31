#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


// int pthread_create(pthread_t *thread, const pthread_attr_t * attr, void *(*start_routine)(void*), void * arg);
// 1. pointer to the thread structure
// 2. thread specific attributes (priority)
// 3. pointer to a function:
// 	void * start_routine (void * params);
// 4. parameters for start_routine function

// void pthread_exit(void *value_ptr);
// pthread_t pthread_self();
// int pthread_join(pthread_t thread, void ** value_ptr);


void * f(void *args){
	void * a = (void*)malloc(sizeof(pthread_t));
	int i;
	pthread_t id = pthread_self();
	a = &id;
	printf("Printing from the thread. ID = %d\n",(int)id);
	return a;
}

void * g(void *args){
	void * a = (void*)malloc(sizeof(pthread_t));
	pthread_t id = pthread_self();
	a = &id;
	printf("2-> Printing from the thread. ID = %d\n",(int)id);
	return a;
}


int main(int argc, char const *argv[])
{
	pthread_t my_thread;
	pthread_t my_thread2;
	pthread_create(&my_thread, NULL, f, NULL);
	pthread_create(&my_thread2, NULL, g, NULL);
	pthread_join(my_thread2,NULL);
	pthread_join(my_thread,NULL);
	return 0;
}

