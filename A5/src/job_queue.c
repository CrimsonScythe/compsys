#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "job_queue.h"

pthread_mutex_t pop_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t pop_cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t push_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t push_cond = PTHREAD_COND_INITIALIZER;

int job_queue_init(struct job_queue *job_queue, int capacity) {
  // job_queue->data = malloc(sizeof(void) * capacity);
  job_queue->data = malloc(sizeof(int) * capacity);
  job_queue->front = 0;
  job_queue->end = 0;
  job_queue->capacity = capacity;
  job_queue->isEmpty=1;
  job_queue->isFull=0;
  job_queue->filling = 0;

  return 0;
}

int job_queue_destroy(struct job_queue *job_queue) {
  // while (job_queue->isEmpty==0)
  // {
    // pthread_cond_wait()
  // }
  
  return(0);
}

int job_queue_push(struct job_queue *job_queue, void* data) {

  assert(pthread_mutex_lock(&push_mutex)==0);
  
  while (job_queue->isFull==1) {
    pthread_cond_wait(&push_cond, &push_mutex);
  }
  // printf();
  job_queue->data[job_queue->front] = *(data); 
  job_queue->front = (job_queue->front+1) % job_queue->capacity;
  job_queue->filling++;
  if (job_queue->filling != 0) {
    job_queue->isEmpty=0;
  }
  if (job_queue->filling == job_queue->capacity) {
    job_queue->isFull=1;
  }
  // onyl send signal if isempty??
  pthread_cond_signal(&pop_cond);
  assert(pthread_mutex_unlock(&push_mutex)==0);
  
  return(0);
}

int job_queue_pop(struct job_queue *job_queue, void **data) {
  // printf("%s", "ss");

  assert(pthread_mutex_lock(&pop_mutex)==0);
  while (job_queue->isEmpty==1) {
    pthread_cond_wait(&pop_cond, &pop_mutex);
  }
  
  // sleep(5);
  *((int**)data) = malloc(sizeof(int)); 
  **((int**)data) = job_queue->data[job_queue->end];
  job_queue->filling--;
  if (job_queue->filling == 0){
    job_queue->isEmpty=1;
  }
  job_queue->data[job_queue->end] = NULL;
  job_queue->end = (job_queue->end+1) % job_queue->capacity;

  pthread_cond_signal(&push_cond);
  assert(pthread_mutex_unlock(&pop_mutex)==0);

  return(0);
}