#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

//define request buffer, which contains the requestId, the max size of requestBuffer, the array to hold the data, and the current number of data in the buffer
struct requestBuffer {
	long requestId;
	long requestBufferSize;
	long *a;
	long numDataInBuffer;
	time_t startTime;
};

sem_t mutex;

void* consume(void *input) {

	//Locl variables for each slave.
	long numDataInBuffer = 0;
	long size = 0;
	long *a;
	time_t startTime;

	while (1) {
		//acquire lock & enter critical section
		sem_wait(&mutex);

		//initialize local variables using the shared requestBuffer struct
		size = ((struct requestBuffer*) input)->requestBufferSize;
		a = ((struct requestBuffer*) input)->a;
		numDataInBuffer = ((struct requestBuffer*) input)->numDataInBuffer;
		startTime = ((struct requestBuffer*) input)->startTime;

		//if requestBuffer is empty, sleep
		while (numDataInBuffer == 0) {
			sem_wait(&mutex);
		}



		//Taking a request from the queue
		long sleepTime;
		long requestGenerated;
		//time_t requestTaken;
		long *a;
		long requestId;
		numDataInBuffer = ((struct requestBuffer*) input)->numDataInBuffer;
		a = ((struct requestBuffer*) input)->a;
		requestId = a[0];
		sleepTime = a[1];
		requestGenerated = a[2];


		time_t tempTime;
		struct tm *information;
		time(&tempTime);
		information = gmtime(&tempTime);
		pthread_t threadId = pthread_self();
		printf("Slave_%d ", threadId);
		printf("started\t| %4ld | @ time %02d:%02d:%02d UTC\t|\n", requestId,
				(information->tm_hour) % 24, information->tm_min, information->tm_sec);
		fflush(stdout);

		//printf("Slave_%d started\t| %4ld\n", pthread_self(), requestId);

		//Shift the array position by 3 to update the queue.
		for (int i = 0; i < size - 3; i++) {
			a[i] = a[i + 3];
		}

		//Update the number of items in the array to reflect the recently removed request.
		numDataInBuffer -= 3;

		//Thread updates the struct
		((struct requestBuffer*) input)->a = a;
		((struct requestBuffer*) input)->numDataInBuffer = numDataInBuffer;

		// Wake up other threads, give up lock, and sleep.
		sem_post(&mutex);
		sem_wait(&mutex);
		sleep(sleepTime);
		fflush(stdout);

		//>>>>>>>>>>>>>>>>>>>>>>>>>
		time_t rawtime;
		struct tm *info;
		time(&rawtime);
		info = gmtime(&rawtime);

		//>>>>>>>>>>>>>>>>>>>>>>>>>
		printf("Slave_%d ", threadId);
		fflush(stdout);
		printf("consumed\t| %4ld | sleepDuration: %3ld sec | Request completed @ %02d:%02d:%02d UTC\n",
				requestId, sleepTime, (info->tm_hour) % 24,
				info->tm_min, info->tm_sec);
		fflush(stdout);

		sem_post(&mutex);

	}

}

void* produce(void *input) {
	long numDataInBuffer = 0;
	long request = 0;
	long size = 0;
	long *a;
	time_t startTime;


	while (1) {
		//acquire lock & enter critical section
		sem_wait(&mutex);

		//Load local variables from struct
		request = ((struct requestBuffer*) input)->requestId;
		size = ((struct requestBuffer*) input)->requestBufferSize;
		a = ((struct requestBuffer*) input)->a;
		numDataInBuffer = ((struct requestBuffer*) input)->numDataInBuffer;
		startTime = ((struct requestBuffer*) input)->startTime;


		puts("+--------------------------------------------------------------------------------------------------------------+");
		//Continue adding request as long as queue is not full
		while (numDataInBuffer != size) {
			for (int i = numDataInBuffer; i < size; i += 3) {
				//4 variables below only for the purpose of printing UTC time
				time_t rawtime;
				struct tm *info;
				time(&rawtime);
				info = gmtime(&rawtime);

				a[i] = request;
				a[i + 1] = ((rand() % (10)) + 1);
				a[i + 2] = time(NULL) - startTime;
				request++;
				printf("Master        produced\t| %4ld | @ time %02d:%02d:%02d UTC\t|\n", a[i], (info->tm_hour) % 24,
						info->tm_min, info->tm_sec);
				fflush(stdout);
				numDataInBuffer += 3;
			}
		}
		puts("+--------------------------------------------------------------------------------------------------------------+");
		//Master updates struct
		((struct requestBuffer*) input)->requestId = request;
		((struct requestBuffer*) input)->a = a;
		((struct requestBuffer*) input)->numDataInBuffer = numDataInBuffer;

		// Wake up other threads, give up lock, and sleep.

		sem_post(&mutex);
		sem_wait(&mutex);
		sleep(1);
		sem_post(&mutex);

	}

}

int main() {

	//initialize struct and all variables to be used by the queue
	time_t startTime = time(NULL);
	int size = 5;
	size = size * 3;
	struct requestBuffer *info = (struct requestBuffer*) malloc(
			sizeof(struct requestBuffer));
	info->requestBufferSize = size;
	info->requestId = 0;
	info->a = (long*) malloc(size * sizeof(long));
	info->numDataInBuffer = 0;
	info->startTime = startTime;

	sem_init(&mutex, 0, 1);
	pthread_t master, thread1, thread2;

	//master thread
	pthread_create(&master, NULL, produce, (void*) info);

	//slave threads
	pthread_create(&thread1, NULL, consume, (void*) info);
	pthread_create(&thread2, NULL, consume, (void*) info);
	pthread_join(master, NULL);
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	//Deallocate mutex
	sem_destroy(&mutex);
	return 0;
}

