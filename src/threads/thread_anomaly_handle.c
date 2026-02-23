/**
 * @file threads.c
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
 * @brief Thread 4: Handle detected anomalies (event-driven -> triggered by anomaly_detector)
 * 
 * 
 * 
*/
void anomaly_handle(void) 
{
    while (1) {
        log_msg_t msg = {.thread_id = 4, .message = "In thread 4"};
        k_msgq_put(&log_queue, &msg, K_NO_WAIT);
        k_msleep(5000);
    }
}