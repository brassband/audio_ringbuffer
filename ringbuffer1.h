#ifndef RINGBUFFER1_H
#define RINGBUFFER1_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int init_ringbuffer(const int n);
void enqueue(const float value);
float dequeue();
void deinit_ringbuffer();

#endif // RINGBUFFER1_H
