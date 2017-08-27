#include "ringbuffer1.h"

// https://stackoverflow.com/questions/1856599/when-to-use-static-keyword-before-global-variables
// Globals in this file
static pthread_mutex_t lock;
static sem_t semcount, semspace; 
static float * buffer;
static int in, out;
static int bufferSize;

int init_ringbuffer(int n) {
	in = 0;
	out = 0;
	bufferSize = n;

	if (pthread_mutex_init(&lock, NULL)) {
		printf("pthread_mutex_init failed, terminating.\n");
		return -1;
	}

	buffer = (float *) malloc(sizeof(float) * n);
	if (buffer == NULL) {
		return -1;
	}

	if (sem_init(&semcount, 0, 0)) {
		printf("sem_init(&semcount, 0, 0) failed, terminating.\n");
		return -1;
	}

	if (sem_init(&semspace, 0, n)) {
		printf("sem_init(&semspace, 0, 0) failed, terminating.\n");
		return -1;
	}
	return 0;
}

void enqueue(float value) {
	sem_wait(&semspace);		// check that there is space in the ringbuffer to write another number
	pthread_mutex_lock(&lock);	// lock the resourse
	buffer[in++] = value;
	if (in == bufferSize) {
		in = 0;
	}
	pthread_mutex_unlock(&lock);// unlock the resource
	sem_post(&semcount);		// increment the count of number of items
}

float dequeue() {
	float value;
	sem_wait(&semcount);		// check that there is a number available in the ringbuffer
	pthread_mutex_lock(&lock);	// lock the resource
	value = buffer[out++];
	if (out == bufferSize) {
		out = 0;
	}
	pthread_mutex_unlock(&lock);// unlock the resource
	sem_post(&semspace);		// increase number of available space counter by one

	return value;
}

void deinit_ringbuffer() {
	sem_destroy(&semcount);
	sem_destroy(&semspace);
	pthread_mutex_destroy(&lock);
}