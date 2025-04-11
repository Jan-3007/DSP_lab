// example for a circular buffer
// NOTE: this is only an example
//
// buffer description:
// get_ptr functions are for the DMA only!
// for read/write by the user, call standard read/write functions
//



// initialize buffers
void init_buffer();


// functions for RX buffer

// read block of samples of BLOCK_SIZE from RX buffer
// returns false if there is nothing to read
bool rx_buffer_read(uint32_t output[BLOCK_SIZE]);

// returns a ptr to the RX buffer for writing a block of samples of BLOCK_SIZE (for DMA)
// increments write offset
uint32_t* rx_buffer_get_write_ptr();


// functions for TX buffer

// write block of samples of BLOCK_SIZE to TX buffer
// returns false if data cannot be written
bool tx_buffer_write(const uint32_t input[BLOCK_SIZE]);

// returns a ptr to the TX buffer for reading a block of samples of BLOCK_SIZE (for DMA)
// increments read offset
uint32_t* tx_buffer_get_read_ptr();
