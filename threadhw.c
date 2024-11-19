#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define NUM_THREADS 10

typedef struct {
    int start;
    int end;
    long sum;
    int thread_id;
} ThreadData;

void* add_numbers(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    data->sum = 0;
    for (int i = data->start; i <= data->end; i++) {
        data->sum += i;
    }
    printf("Thread %d finished with sum: %ld\n", data->thread_id, data->sum);
    pthread_exit(NULL);
}

long single_thread_sum(int upper_limit) {
    long sum = 0;
    for (int i = 1; i <= upper_limit; i++) {
        sum += i;
    }
    return sum;
}

int main(int argc, char* argv[]) {
    int upper_limit = 100;
    if (argc > 1) {
        upper_limit = atoi(argv[1]);
    }

    // Calculate range per thread
    int range_per_thread = upper_limit / NUM_THREADS;
    ThreadData thread_data[NUM_THREADS];
    pthread_t threads[NUM_THREADS];
    long total_sum = 0;

    // Measure time for multithreaded solution
    clock_t start_time = clock();
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].start = i * range_per_thread + 1;
        thread_data[i].end = (i == NUM_THREADS - 1) ? upper_limit : (i + 1) * range_per_thread;
        thread_data[i].thread_id = i + 1;
        pthread_create(&threads[i], NULL, add_numbers, &thread_data[i]);
    }

    // Join threads and accumulate results
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        total_sum += thread_data[i].sum;
    }
    clock_t end_time = clock();
    double multithread_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Multithreaded total sum: %ld\n", total_sum);
    printf("Multithreaded time: %f seconds\n", multithread_time);

    // Measure time for single-threaded solution
    start_time = clock();
    long single_thread_total = single_thread_sum(upper_limit);
    end_time = clock();
    double single_thread_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Single-threaded total sum: %ld\n", single_thread_total);
    printf("Single-threaded time: %f seconds\n", single_thread_time);

    // Compare times
    printf("Time difference (multi - single): %f seconds\n", multithread_time - single_thread_time);

    return 0;
}