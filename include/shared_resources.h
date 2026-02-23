#ifndef SHARED_RESOURCES_H
#define SHARED_RESOURCES_H

/**
 * @file shared_resources.h
 * @brief Shared data structures and definitions for inter-thread communication.
*/

#include <zephyr/kernel.h>

#define LOG_MSG_SIZE                64
#define LOG_QUEUE_SIZE              16

/** @brief Memory alignment for the message queue buffer in bytes */
#define MESSAGE_ALIGN               4

/** @brief Global message queue for serialized terminal logging  */
extern struct k_msgq log_queue;

extern struct k_mutex sensor_mutex;
extern struct k_mutex buffer_mutex;

/** 
 * @brief Log message structure for inter-thread communication
 * 
 * Passed through the logging message queue from threads 1-4
 * to the system logger (thread 5) for terminal output.
*/
typedef struct {
    int thread_id;                      /**< ID of the thread that emitted the message */
    char message[LOG_MSG_SIZE];         /**< Null-terminated log message string */
} log_msg_t;

/**
 * @brief Represents a single sensor reading from a machine.
 * 
 * Stores the machine and sensor identity alongside the recorded value
 * and its valid operating range. Used as the unit of data passed
 * through the circular buffer from Thread 2 to Thread 3.
*/
struct sensor_reading { 
    char machine_name[16];      /**< Name of the machine this reading belongs to */
    char sensor_type[16];       /**< Name of the sensor that produced the reading */
    float value;                /**< Recorded sensor value */
    float min_value;            /**< Minimum value of the sensor's valid operating range */
    float max_value;            /**< Maximum value of the sensor's valid operating range */
};

#endif // SHARED_RESOURCES_H