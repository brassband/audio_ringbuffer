#include <stdio.h>
#include <pthread.h>
#include "ringbuffer1.h"
#include "audiohandler.h"

#define N 4

void * reader(void * fName) {
	char * fileName = (char *) fName;
	audioInfo_t myAudioInfo;
	float * myAudioData;

	if (getAudioMetaData(fileName, &myAudioInfo)) {
		printf("getAudioMetaData() failed. Terminating.\n");
		goto exit;
	}

	int numSamples = myAudioInfo.numChannels * myAudioInfo.numFrames;
	myAudioData = malloc(sizeof(float) * numSamples);
	if (!myAudioData) {
		printf("malloc() failed. Terminating.\n");
		goto exit;
	}

	if (readAudioData(fileName, myAudioInfo, myAudioData)) {
		printf("readAudioData() failed. Terminating.\n");
		goto exit;
	}

	printf("numSamples = %d\n", numSamples);

	int i = 0;
	while (i < numSamples) {
		enqueue(myAudioData[i++]);
	}

exit:
	if (myAudioData)
		free(myAudioData);
	printf("Leaving reader thread.\n");
}

void * writer() {
	int i;
	float value;
	for (i = 0; i < 40; i++) {
		value = dequeue();
		printf("value = %.2f\n", value);
	}
	printf("Leaving writer thread.\n");
}

int main() {
	char fileName[] = "20samples_stereo.wav";
	pthread_t readerthread, writerthread;

	if (init_ringbuffer(N)) {
		printf("init_ringbuffer() failed. Terminating.\n");
		return -1;
	}

	if (pthread_create(&readerthread, NULL, reader, (void *) fileName)) {
		printf("pthread_init readerthread failed. Terminating.\n");
		return -1;
	}

	if (pthread_create(&writerthread, NULL, writer, NULL)) {
		printf("pthread_init readerthread failed. Terminating.\n");
		return -1;
	}

	pthread_join(readerthread, NULL);
	pthread_join(writerthread, NULL);

	printf("HEJ\n");

	return 0;
}