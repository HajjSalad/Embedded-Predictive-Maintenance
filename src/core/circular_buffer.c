/**
* @file circular_buffer.c
* @brief Circular buffer implementation for storing sensor data.
*
* This module provides a FIFO circular buffer with overwrite-on-full behaviour.
* It is intended for producer-consumer pipelines where old sensor data is dropped
* when the buffer reaches capacity. 
*/

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "circular_buffer.h"

/**
* @brief Initialize the circular buffer
*
* Sets the head and tail indices to zero, marking the buffer as empty.
*
* @param cb Pointer to the CircularBuffer instance.
*/
void circular_buffer_init(CircularBuffer *cb) 
{
    if (cb == NULL) {
        return;
    }

    cb->head = 0U;
    cb->tail = 0U;
}

/**
* @brief Write a sensor reading into the circular buffer by value.
* 
* Copies the sensor reading into the buffer at the current head position.
* If the buffer is full, the oldest entry is overwritten by advancing
* the tail index.
*
* @param cb      Pointer to the CircularBuffer instance.
* @param reading Pointer to the sensor_reading to copy into the buffer.
*
* @return void
*/
void cb_write(CircularBuffer *cb, const struct sensor_reading *reading)
{
    if (cb == NULL || reading == NULL) {
        return;
    }

    // Copy the sensor reading into he buffer at current write position
    (void)memcpy(&cb->entries[cb->head], reading, sizeof(struct sensor_reading));

    // Advance head index (wrap around at BUFFER_SIZE)
    cb->head = (cb->head + 1U) % BUFFER_SIZE;

    // If head catches up to tail, buffer was full - Advance tail to discard the oldes entry 
    if (cb->head == cb->tail) {
        cb->tail = (cb->tail + 1U) % BUFFER_SIZE;
    }
}

/**
* @brief Read a sensor reading from the circular buffer by value
* 
* Copies the oldest sensor reading into the provided output struct
* in FIFO order.
*
* @param cb     Pointer to the CircularBuffer instance.
* @param output Pointer to a sensor_reading struct to receive the copy.
*
* @return true  if sensor reading successfully read
* @return false if the buffer was empty or a NULL pointer was provided
*/
bool cb_read(CircularBuffer *cb, struct sensor_reading *output) 
{
    if (cb == NULL || output == NULL) {
        return false;
    }

    // Buffer is empty if head equals tail
    if (cb->head == cb->tail) {
        return false;
    }

    // Retrieve the oldest reading
    (void)memcpy(output, &cb->entries[cb->tail], sizeof(struct sensor_reading));

    // Advance tail index
    cb->tail = (cb->tail + 1U) % BUFFER_SIZE;

    return true;
}

