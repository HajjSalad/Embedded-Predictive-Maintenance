#ifndef THREADS_H
#define THREADS_H

/**
 * @file threads.h
 * @brief 
*/

#include <zephyr/kernel.h>

/** @brief Size of each log message in bytes */
#define LOG_MSG_SIZE                64

/** @brief Max number of log messages in the queue */
#define LOG_QUEUE_SIZE              16

/** @brief Memory alignment for the message queue buffer in bytes */
#define MESSAGE_ALIGN               4

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

/** @brief Global message queue for serialized terminal logging  */
extern struct k_msgq log_queue;

// Function Prototypes
void sensor_data_writer(void);
void data_collector(void);
void anomaly_detector(void);
void anomaly_handler(void);
void system_logger(void);

#endif // THREADS_H