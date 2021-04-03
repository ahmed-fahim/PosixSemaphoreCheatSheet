/**
	A C-Posix Semaphore Cheat-Sheet

	- For brief introduction to Posix Semaphores  :
		https://www.softprayog.in/programming/posix-semaphores

	Written By:
		Fahim Ahmed
		Senior Software Engineer
		Department of Solution Engineering & Architecture,
		Product & Technology Division
		bKash Ltd.
*/


/*
 * The following source code has been written with
 * "Max 80 chars per line coding convention"
 */

#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

//In this case, at a time, only 1 process should get access to the semaphore
#define SEMAPHORE_PARALLEL_ACCESS_COUNT 1

enum SEMAPHORE_SHARE_MODE {
	SHARE_AMONG_THREADS = 0,
	SHARE_AMONG_PROCESSES = 1
};

//Declaration for the function that would print in a synchronized manner
void synchronizedPrinter(sem_t *mutex);

int main() {
	/**
	 * The semaphore object that will be used as a mutex in our case. Mutex is
	 * a binary locking mechanism. At a time only one process/thread can have
	 * access to a mutex object.
	 * */
	sem_t mutex;

	//Initiating the semaphore as a mutex object
	sem_init(&mutex,
	         SHARE_AMONG_PROCESSES,
	         SEMAPHORE_PARALLEL_ACCESS_COUNT);

	/**
	 * DOCUMENTATION: How to count total processes created by fork()?
	 * --------------------------------------------------------------
	 * Each fork causes the current number of processes to go double.
	 * Suppose we have 1 process "root". Now "root" executes a fork().
	 * This will give us 1 extra process "root-child".
	 * So now we have 2 processes.
	 * (1) "root", (2) "root-child".
	 *
	 * Now if the "root" process had another fork(), it's child process would
	 * also have this fork() in it's program execution path.
	 * Hence, "root" creates another process "root-child2"
	 * And, "root-child" creates another process "root-child-child".
	 * So now we have 4 process.
	 * (1) "root", (2) "root-child", (3) "root-child2", (4) "root-child-child"
	 *
	 * Hence for every consecutive fork() we get 2 times more process.
	 * So, for N consecutive fork(), we will get a total of 2^N processes.
	**/

	//To have 4 processes we need log2(4) = 2 forks
	fork();
	fork();
	synchronizedPrinter(&mutex);
	synchronizedPrinter(&mutex);

}

//Definition for the function that would print in a synchronized manner
void synchronizedPrinter(sem_t *mutex) {
	//Waiting on semaphore object until it is free to use.
	sem_wait(mutex);

	printf("\n%s() :: semaphore access owned by Process-Id %d\n",
	       __func__, (int) getpid());

	//printing the process Id 2 times
	for (int i = 0; i < 2; i++) {
		printf("Hello World from processId = %d\n", (int) getpid());
	}

	printf("%s() :: semaphore access being released by Process-Id %d\n",
	       __func__, (int) getpid());

	//After semaphore use is done, do sem_post() to increment the value so
	// another waiting process (among all waiting processes) can use it
	sem_post(mutex);
}