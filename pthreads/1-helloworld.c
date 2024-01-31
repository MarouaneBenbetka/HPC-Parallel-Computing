#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

pthread_t thread1, thread2; // Thread identifiers (global variables - Visible to all threads -)

void *my_proc(void *ptr)
{
    char *message;
    message = (char *)ptr; // Casting the parameter to the appropriate type...

    pthread_t tid = pthread_self(); // Returns the identifier of the calling thread (thread 1 or thread 2)

    int test = pthread_equal(tid, thread1); // Tests if the identifiers are equal (Is it thread 1 or thread 2?)

    if (test == 0) // When the threads are different, 0 is returned (it's thread 2)
    {
        pthread_join(thread1, NULL);
        printf("%s ", message);
    }
    else // When the identifiers are the same, a non-zero value is returned (it's thread 1)
    {
        printf("%s ", message);
    }
}

int main()
{
    char *m1 = "Hello";
    char *m2 = "World";

    pthread_create(&thread1, NULL, my_proc, (void *)m1);
    pthread_create(&thread2, NULL, my_proc, (void *)m2);

    pthread_exit(NULL);
}
