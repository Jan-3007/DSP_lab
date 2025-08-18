/*
 * Author: Jan Eberhardt
 */

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
    // lock out ISR, a Mutex or Semaphore can be used as well
    NVIC_DisableIRQ(DSTC_HW_IRQn);

    // check if buffer is empty
    if(read_ptr_ == write_ptr_ && !buffer_full_)
    {
        // nothing to read
        
        // unlock ISR
        NVIC_EnableIRQ(DSTC_HW_IRQn);
        
        return false;
    }

    // unlock ISR
    NVIC_EnableIRQ(DSTC_HW_IRQn);

    // buffer has data
    // copy data from buffer to output
    for(uint32_t n = BLOCK_SIZE; n > 0; n--)
    {
        // copy sample
        *output++ = *read_ptr_++;
    }

    // check if end of buffer is reached
    if(read_ptr_ >= buffer_ + BUFFER_SIZE)
    {
        read_ptr_ = buffer_;
    }

    // buffer after read not full anymore
    buffer_full_ = false;

    return true;
}


uint32_t*
CircularBuffer::get_read_ptr()
{
    // this check is required, otherwise a fatal error will be thrown when the DMA gets its first ptr as part of its setup
    if(!initial_setup_)
    {
        // normal operation

        // increment read_ptr_, because DMA can/will not do it
        read_ptr_ = read_ptr_ + BLOCK_SIZE;

        // check if end of buffer is reached
        if(read_ptr_ >= buffer_ + BUFFER_SIZE)
        {
            read_ptr_ = buffer_;
        }

        // buffer after read not full anymore
        buffer_full_ = false;
    
        // check if buffer is empty
        if(read_ptr_ == write_ptr_ && !buffer_full_)
        {
            // nothing to read
            fatal_error();
        }

        return read_ptr_;
    }
    else
    {
        // initial setup

        // assert end of buffer is not reached
        ASSERT(read_ptr_ < buffer_ + BUFFER_SIZE);

        initial_setup_ = false;

        // get read pointer, but not increment it
        // read pointer will be incremented once get_read_ptr() is called again
        // this is to only increment the read pointer after the DMA has read data from the buffer
        return read_ptr_;
    }
}


bool 
CircularBuffer::write(uint32_t input[BLOCK_SIZE])
{
    // lock out ISR, a Mutex or Semaphore can be used as well
    NVIC_DisableIRQ(DSTC_HW_IRQn);

    // check if buffer is full
    if(write_ptr_ == read_ptr_ && buffer_full_)
    {
        // cannot write

        // unlock ISR
        NVIC_EnableIRQ(DSTC_HW_IRQn);

        return false;
    }

    // copy data from input to buffer
    for(uint32_t n = BLOCK_SIZE; n > 0; n--)
    {
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

    // unlock ISR
    NVIC_EnableIRQ(DSTC_HW_IRQn);

    return true;
}


uint32_t*
CircularBuffer::get_write_ptr()
{
    // this check is required, otherwise a fatal error will be thrown when the DMA gets its first ptr as part of its setup
    if(!initial_setup_)
    {
        // normal operation

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

        // check if buffer is full
        if(buffer_full_)
        {
            // cannot write
            fatal_error();
        }

        return write_ptr_;
    }
    else
    {
        // initial setup

        // assert end of buffer is not reached
        ASSERT(write_ptr_ < buffer_ + BUFFER_SIZE);


        initial_setup_ = false;

        // get write pointer, but not increment it
        // write pointer will be incremented once get_write_ptr() is called again
        // this is to only increment the write pointer after the DMA has written its data to the buffer
        return write_ptr_;
    }
}

