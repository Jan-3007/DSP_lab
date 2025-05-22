#pragma once


// GPIO
#define HIGH 1
#define LOW 0
#define ENABLE 1
#define DISABLE 0



// must be between 1 - 256 (inclusive) 
// (or multiple of 256 (e.g. 512, 768, 1024, ...))
#define BLOCK_SIZE 64 



// FiFo Buffer
#define FIFO_SIZE_BLOCKS 3
#define BUFFER_SIZE (FIFO_SIZE_BLOCKS * BLOCK_SIZE)

