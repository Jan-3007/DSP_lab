#include "global.h"


// example for a circular buffer
// NOTE: this is only an example

// RX and TX buffer
#define BUFFER_SIZE FIFO_SIZE_BLOCKS * BLOCK_SIZE
uint32_t rx_buffer[BUFFER_SIZE];
uint32_t tx_buffer[BUFFER_SIZE];

// read and write offsets
uint32_t rx_read_offset = 0;
uint32_t rx_write_offset = 0;
uint32_t tx_read_offset = 0;
uint32_t tx_write_offset = 0;


void init_buffer()
{
    rx_write_offset = 1;
    tx_read_offset = 1;

    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(tx_buffer, 0, sizeof(tx_buffer));
}


// functions for RX buffer

// read block of samples of BLOCK_SIZE from RX buffer
// returns false if there is nothing to read
bool rx_buffer_read(uint32_t output[BLOCK_SIZE])
{
    // check if read is overrunning write
    if((rx_read_offset % FIFO_SIZE_BLOCKS) == rx_write_offset)
    {
        // RX buffer empty, nothing to read
        return false;
    }

    uint32_t i = BLOCK_SIZE;
    uint32_t offset = rx_read_offset * BLOCK_SIZE;
    uint32_t* dest = output;

    // loop over BLOCK_SIZE
    while (i > 0)
    {
      // copy sample from the circular buffer to the destination buffer
      *dest++ = rx_buffer[offset++];

      // decrement the loop counter
      i--;
    }

    rx_read_offset++;
    if (rx_read_offset >= FIFO_SIZE_BLOCKS)
    {
        rx_read_offset = 0;
    }

    return true;
}


// returns a ptr to the RX buffer for writing a block of samples of BLOCK_SIZE (for DMA)
// increments write offset
uint32_t* rx_buffer_get_write_ptr()
{
    // check if write is overrunning read
    if((rx_write_offset % FIFO_SIZE_BLOCKS) == rx_read_offset)
    {
        // rx buffer full, cannot write
        fatal_error();
    }

    // calculate new write ptr
    uint32_t* ptr = rx_buffer + (rx_write_offset * BLOCK_SIZE);

    // increment write offset for next time
    rx_write_offset++;
    if (rx_write_offset >= FIFO_SIZE_BLOCKS)
    {
        rx_write_offset = 0;
    }

    return ptr;
}



// functions for TX buffer

// write block of samples of BLOCK_SIZE to TX buffer
// returns false if data cannot be written
bool tx_buffer_write(const uint32_t input[BLOCK_SIZE])
{
    // check if write is overrunning read
    if((tx_write_offset % FIFO_SIZE_BLOCKS) == tx_read_offset)
    {
        // TX buffer full, cannot write
        return false;
    }

    uint32_t i = BLOCK_SIZE;
    uint32_t offset = tx_write_offset * BLOCK_SIZE;
    const uint32_t* src = input;

    // loop over BLOCK_SIZE
    while (i > 0)
    {
      // copy sample from the  buffer to the circular buffer
      tx_buffer[offset++] = *src++;

      // decrement the loop counter
      i--;
    }

    tx_write_offset++;
    if (tx_write_offset >= FIFO_SIZE_BLOCKS)
    {
        tx_write_offset = 0;
    }

    return true;
}


// returns a ptr to the TX buffer for reading a block of samples of BLOCK_SIZE (for DMA)
// increments read offset
uint32_t* tx_buffer_get_read_ptr()
{
    // check if read is overrunning write
    if((tx_read_offset % FIFO_SIZE_BLOCKS) == tx_write_offset)
    {
        // tx buffer emty, nothing to read
        fatal_error();
    }

    // calculate new read ptr
    uint32_t* ptr = tx_buffer + (tx_read_offset * BLOCK_SIZE);

    // increment write offset for next time
    tx_read_offset++;
    if (tx_read_offset >= FIFO_SIZE_BLOCKS)
    {
        tx_read_offset = 0;
    }

    return ptr;
}
