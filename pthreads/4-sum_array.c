#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct
{ //
    int nb;
    long *t;
} Tdonnees;

void *somme(void *donnees)
{
    Tdonnees *d = (Tdonnees *)donnees;
    int i;
    long total = 0;
    for (i = 0; i < d->nb; i++)
    {
        total += (d->t)[i];
    }
    pthread_exit((void *)total);
}

int main()
{
    pthread_t thread1, thread2, thread3;

    long tab[15] = {8, 3, 9, 5, 7, 12, 7, 45, 9, 3, 45, 12, 6, 6, 9};

    Tdonnees d1, d2, d3;
    /
        long total = 0;
    void *v1, *v2, *v3;

    d1.nb = 5;
    d1.t = &(tab[0]);
    d2.nb = 5;
    d2.t = &(tab[5]);
    d3.nb = 5;
    d3.t = &(tab[10]);

    pthread_create(&thread1, NULL, somme, (void *)&d1);
    pthread_create(&thread2, NULL, somme, (void *)&d2);
    pthread_create(&thread3, NULL, somme, (void *)&d3);

    pthread_join(thread1, &v1);
    pthread_join(thread2, &v2);
    pthread_join(thread3, &v3);

    total = (long)v1 + (long)v2 + (long)v3;

    printf("SUM %ld\n", total);

    return 0;
}