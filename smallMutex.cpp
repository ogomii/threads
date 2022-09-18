#include <chrono>
#include <iostream>
#include <pthread.h>

#define NUM_OF_THREADS 2

/*
  mutex lock only for adding result to shared variable
  elapsedTime averaged around 1.5 s
*/
pthread_mutex_t mutex;

void *add(void *input) {
  int a = 0;

  for (int i = 0; i < 1e9; i++) {
    a += 1;
  }

  // std::cout << "Result of thread (id: " << pthread_self() << " ) : " << *a
  // << std::endl;

  pthread_mutex_lock(&mutex);
  *(int *)input += a;
  pthread_mutex_unlock(&mutex);

  return NULL;
}

int main(void) {
  pthread_t threads[NUM_OF_THREADS];
  int result = 0;

  std::cout << "Starting threads" << std::endl;

  auto startTime = std::chrono::high_resolution_clock::now();

  pthread_create(&threads[0], NULL, add, &result);
  pthread_create(&threads[1], NULL, add, &result);

  pthread_join(threads[0], NULL);
  pthread_join(threads[1], NULL);

  auto endTime = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsedTime = endTime - startTime;

  std::cout << "Threads finished, result = " << result << std::endl;
  std::cout << "in time: " << elapsedTime.count() << std::endl;
}
