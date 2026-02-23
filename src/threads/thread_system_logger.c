/**
 * @file thread_system_logger.c
 * @brief
 * 
 * Description
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zephyr/kernel.h>

#include "threads.h"
#include "shared_resources.h"

/**
 * @brief Thread 5: Consume log messages from the logging queue and print to terminal
 * 
 * 
 * 
*/
void system_log(void) 
{
    log_msg_t msg;

    while (1) {
        k_msgq_get(&log_queue, &msg, K_FOREVER);
        printk("Thread %d: %s\n", msg.thread_id, msg.message);
    }
}