#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 9
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

struct Data
{
    long *a;
    long *b;
    long nb;
} typedef Data;

void *f(void *arg)
{
    long *a;
    long *b;
    long nb;
    Data *_data = (Data *)arg;
    a = _data->a;
    b = _data->b;
    nb = _data->nb;
    _data->a = _data->a + nb;
    _data->b = _data->b + nb;
    pthread_mutex_unlock(&mutex1);
    long sum = 0;
    for (int i = 0; i < nb; i++)
        sum += a[i] * b[i];
    return (void *)sum;
}

int main(int argc, char **argv)
{
    float timeIns;
    clock_t t1, t2;
    long size;
    pthread_t threads[NUM_THREADS];
    Data _data;
    printf("Enter Vector size :");
    scanf("%ld", &size);

    long sum, a[size], b[size];
    sum = 0;
    t1 = clock();
    for (size_t i = 0; i < size; i++)
    {
        a[i] = i * 5;
        b[i] = i * 2;
    }
    long chunk = size / NUM_THREADS;
    long rest = size % NUM_THREADS;
    _data.a = a;
    _data.b = b;
    _data.nb = chunk;
    for (int i = 0; i < NUM_THREADS - 1; i++)
    {
        pthread_mutex_lock(&mutex1);
        pthread_create(&threads[i], NULL, f, &_data);
    }
    // last one
    pthread_mutex_lock(&mutex1);
    _data.nb += rest;
    pthread_create(&threads[NUM_THREADS - 1], NULL, f, &_data);

    for (int i = 0; i < NUM_THREADS; i++)
    {
        long p_sum = 0;
        pthread_join(threads[i], (void *)&p_sum);
        sum += (long)p_sum;
    }
    t2 = clock();
    timeIns = (float)(t2 - t1) / CLOCKS_PER_SEC;

    printf("time = %f\n", timeIns);
    printf("sum = %ld\n", sum);
    return 0;
}