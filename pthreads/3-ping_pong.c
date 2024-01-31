#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Program allowing the launch of two threads that display
// a message in an infinite loop
// (Ping for one and Pong for the other alternately):

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *ping()
{
    while (1)
    {
        printf("Ping \n");

        pthread_mutex_lock(&lock);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&lock);

        pthread_mutex_lock(&lock);
        pthread_cond_wait(&cond, &lock);
        pthread_mutex_unlock(&lock);
    }
}

void *pong()
{
    while (1)
    {
        pthread_mutex_lock(&lock);
        pthread_cond_wait(&cond, &lock);
        pthread_mutex_unlock(&lock);

        printf("Pong \n");

        pthread_mutex_lock(&lock);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&lock);
    }
}

int main()
{
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, pong, NULL);
    pthread_create(&thread2, NULL, ping, NULL);
    pthread_join(thread1, NULL); // Il est nécessaire que le thread principal ne termine pas le programme tout de suite après la création des threads
    pthread_join(thread2, NULL); // Autrement rien ne s'affiche
    return 0;
}