#include <stdio.h>
#include <pthread.h>
#define NUM_THREADS 4

void *helloworld(void *arg)
{
    int *i = (int *)arg;

    printf("Hello World! Thread %d\n", *i);

    return NULL;
}

int main()
{
    int i;
    int tab[NUM_THREADS] = {0, 1, 2, 3};

    pthread_t threads[NUM_THREADS];

    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, helloworld, (void *)&tab[i]);
    }

    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}