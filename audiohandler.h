#include <stdio.h>
#include <stdint.h>

// defines
// #define INT8_MIN -128
// #define INT16_MIN -32768
// #define INT32_MIN −2147483648

// types
typedef struct audioInfo {
	uint8_t numChannels;
	uint16_t sampleRate;
	uint8_t numBits;
	uint32_t numFrames;
} audioInfo_t;

int getAudioMetaData(char * fileName, audioInfo_t * audioInfo);
int readAudioData(char * fileName, audioInfo_t audioInfo, float * audioData);
int writeAudioData(char * fileName, audioInfo_t audioInfo, float * audioData);