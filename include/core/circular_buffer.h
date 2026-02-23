#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

/**
* @file circular_buffer.h
* @brief Circular buffer interface for storing sensor data.
*/

#include <stdint.h>
#include <stdbool.h>

#include "shared_resources.h"

/** @brief Max number of sensor data entries held in the circular buffer. */
#define BUFFER_SIZE     12U

/**
* @brief Circular buffer for storing sensor readings by value.
*
* - entries: array of sensor_reading structs (owned by the buffer)
* - head:    index for writing new sensor data (producer position)
* - tail:    index for reading sensor data (consumer position)
*/
typedef struct CircularBuffer{
    struct sensor_reading entries[BUFFER_SIZE];     /**< Array of sensor readings stored by value */ 
    uint32_t head;                                  /**< write index */ 
    uint32_t tail;                                  /**< read index */ 
} CircularBuffer;

/** @brief Global circular buffer instance for sensor data exchange between threads */
extern CircularBuffer circular_buffer;  

/** Function prototypes */
void circular_buffer_init(CircularBuffer *cb);
void cb_write(CircularBuffer *cb, const struct sensor_reading* reading);
bool cb_read(CircularBuffer *cb, struct sensor_reading* output);

#endif  // CIRCULAR_BUFFER_H