#include "global.h"


// ctor
CircularBuffer::CircularBuffer()
{
}


void 
CircularBuffer::init()
{
    memset(buffer_, 0, sizeof(buffer_));
}


bool 
CircularBuffer::read(uint32_t output[BLOCK_SIZE])
{
    // check if buffer is empty
    if(read_ptr_ == write_ptr_ && !buffer_full_)
    {
        // nothing to read
        return false;
    }

    // copy data from buffer to output
    for(size_t n = BLOCK_SIZE; n > 0; n--)
    {
        // copy sample
        *output++ = *read_ptr_++;
    }

    // check if end of buffer is reached
    if(read_ptr_ >= buffer_ + BUFFER_SIZE)
    {
        read_ptr_ = buffer_;
    }

    // check if read has caught up to write
    if(read_ptr_ == write_ptr_)
    {
        // buffer is empty
        buffer_full_ = false;
    }

    return true;
}


uint32_t*
CircularBuffer::get_read_ptr()
{
    // this check is required, otherwise a fatal error will be thrown when the DMA gets its first ptr as part of its setup
    if(!initial_setup_)
    {
        // check if buffer is empty
        if(read_ptr_ == write_ptr_ && !buffer_full_)
        {
            // nothing to read
            fatal_error();
        }
    }
    else
    {
        initial_setup_ = false;
    }

    uint32_t* temp = read_ptr_;
    
    // increment read_ptr_, because DMA can/will not do it
    read_ptr_ = read_ptr_ + BLOCK_SIZE;

    // check if end of buffer is reached
    if(read_ptr_ >= buffer_ + BUFFER_SIZE)
    {
        read_ptr_ = buffer_;
    }

    // check if read has caught up to write
    if(read_ptr_ == write_ptr_)
    {
        // buffer is empty
        buffer_full_ = false;
    }

    return temp;
}


bool 
CircularBuffer::write(uint32_t input[BLOCK_SIZE])
{
    // check if buffer is full
    if(buffer_full_)
    {
        // cannot write
        return false;
    }

    // copy data from input to buffer
    for(size_t n = BLOCK_SIZE; n > 0; n--)
    {
        // copy sample
        *write_ptr_++ = *input++;
    }

    // check if end of buffer is reached
    if(write_ptr_ >= buffer_ + BUFFER_SIZE)
    {
        write_ptr_ = buffer_;
    }

    // check if write has caught up to read
    if(write_ptr_ == read_ptr_)
    {
        // buffer is full
        buffer_full_ = true;
    }

    return true;
}


uint32_t*
CircularBuffer::get_write_ptr()
{
    // this check is required, otherwise a fatal error will be thrown when the DMA gets its first ptr as part of its setup
    if(!initial_setup_)
    {
        // check if buffer is full
        if(buffer_full_)
        {
            // cannot write
            fatal_error();
        }
    }
    else
    {
        initial_setup_ = false;
    }

    uint32_t* temp = write_ptr_;

    // increment write_ptr_, because DMA can/will not do it
    write_ptr_ = write_ptr_ + BLOCK_SIZE;

    // check if end of buffer is reached
    if(write_ptr_ >= buffer_ + BUFFER_SIZE)
    {
        write_ptr_ = buffer_;
    }

    // check if write has caught up to read
    if(write_ptr_ == read_ptr_)
    {
        // buffer is full
        buffer_full_ = true;
    }

    return temp;
}
