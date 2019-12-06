#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "job_queue.h"


  pthread_mutex_t q_mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t pop_cond = PTHREAD_COND_INITIALIZER;

  pthread_cond_t push_cond = PTHREAD_COND_INITIALIZER;

  pthread_cond_t destroy_cond = PTHREAD_COND_INITIALIZER;

int job_queue_init(struct job_queue *job_queue, int capacity) {

  (job_queue->data) = malloc(sizeof(void*) * capacity);
  job_queue->front = 0;
  job_queue->end = 0;
  job_queue->capacity = capacity;
  job_queue->isEmpty=1;
  job_queue->isFull=0;
  job_queue->filling = 0;
  job_queue->destroyed = 0;

  return 0;
}

int job_queue_destroy(struct job_queue *job_queue) {
  
  // destroy is called and is now waiting for queue to be empty
  // two pops are waiting, 
  // one is executed and queue becomes empty signalling detroy to wake-up
  // destroy must wake up the one reminaing pop with exit code 1 since no pushes will be executed after destroy

  // if any pops are waiting when destroy wakes up, wake up the pops with exit code 1.

  assert(pthread_mutex_lock(&q_mutex)==0);
  while (job_queue->isEmpty==0)
  {
    pthread_cond_wait(&destroy_cond, &q_mutex);
  }
  // needed to wake up pops
  job_queue->isEmpty = 0;
  job_queue->destroyed = 1;
  
  // used broadcast to signal to all sleeping pop threads
  pthread_cond_broadcast(&pop_cond);

  free(job_queue->data);

  assert(pthread_mutex_unlock(&q_mutex)==0);

  return(0);
}

int job_queue_push(struct job_queue *job_queue, void* data) {

  assert(pthread_mutex_lock(&q_mutex)==0);
  
  while (job_queue->isFull==1) {
    pthread_cond_wait(&push_cond, &q_mutex);
  }

  (job_queue->data)[job_queue->front] = data; 

  job_queue->front = (job_queue->front+1) % job_queue->capacity;
  job_queue->filling++;
  if (job_queue->filling != 0) {
    job_queue->isEmpty=0;
  }
  if (job_queue->filling == job_queue->capacity) {
    job_queue->isFull=1;
  }
  
  pthread_cond_signal(&pop_cond);
  assert(pthread_mutex_unlock(&q_mutex)==0);
  return(0);
}

int job_queue_pop(struct job_queue *job_queue, void **data) {
  assert(pthread_mutex_lock(&q_mutex)==0);
  while (job_queue->isEmpty==1) {
    pthread_cond_wait(&pop_cond, &q_mutex);
  } 

  if (job_queue->destroyed==1)
  {
    // mutex must be unlocked here, otherwise other threads woken up
    // will not be able to enter the code block
    assert(pthread_mutex_unlock(&q_mutex)==0);
    return -1;
  }
  
  *data = job_queue->data[job_queue->end];

  job_queue->filling--;
  if (job_queue->filling == 0){
    job_queue->isEmpty=1;
    pthread_cond_signal(&destroy_cond);
  }
  job_queue->end = (job_queue->end+1) % job_queue->capacity;
  job_queue->isFull = 0;
  pthread_cond_signal(&push_cond);
  
  assert(pthread_mutex_unlock(&q_mutex)==0);

  return(0);
}