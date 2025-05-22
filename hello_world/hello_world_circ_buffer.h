#pragma once


class CircularBuffer
{
protected:
    uint32_t buffer_[BUFFER_SIZE];

    uint32_t* read_ptr_ {buffer_};
    uint32_t* write_ptr_ {buffer_};

    bool buffer_full_ {false};
    bool initial_setup_ {true};

public:
    // ctor
    CircularBuffer();

    void
    init();

    // read a block of samples of BLOCK_SIZE from buffer
    // returns false if there is nothing to read
    bool
    read(uint32_t output[BLOCK_SIZE]);
    
    // returns a ptr to the buffer for reading a block of samples of BLOCK_SIZE (for DMA only)
    uint32_t*
    get_read_ptr();

    // write a block of samples of BLOCK_SIZE to buffer
    // returns false if data cannot be written
    bool
    write(uint32_t input[BLOCK_SIZE]);

    // returns a ptr to the buffer for writing a block of samples of BLOCK_SIZE (for DMA only)
    uint32_t*
    get_write_ptr();
};