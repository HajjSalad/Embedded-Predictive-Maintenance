/**
 * @file thread_anomaly_detect.c
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
#include "circular_buffer.h"

/**
 * @brief Thread 3: Consume data from the circular buffer and perform anomaly detection
 * 
 * 
*/
void anomaly_detect(void)
{
    while (1) {
        log_msg_t msg = {.thread_id = 3, .message = "Reading circular buffer:"};
        k_msgq_put(&log_queue, &msg, K_NO_WAIT);

        struct sensor_reading reading;

        // Acquire mutex before draining the circular buffer
        (void)k_mutex_lock(&buffer_mutex, K_FOREVER);
        
        // Drain the circular buffer and process each sensor reading
        while(cb_read(&circular_buffer, &reading)) 
        {
            // Release mutex
            (void)k_mutex_unlock(&buffer_mutex);
            
            // Log the reading to verify data is corretcly carried from Thread 2
            char buf[LOG_MSG_SIZE];
            snprintf(buf, sizeof(buf), "  %-15s | %-12s = %6.2f [%.2f-%.2f]",
                reading.machine_name, reading.sensor_type,
                (double)reading.value,
                (double)reading.min_value,
                (double)reading.max_value);

            log_msg_t sensor_msg = {.thread_id = 3};
            strncpy(sensor_msg.message, buf, LOG_MSG_SIZE - 1);
            k_msgq_put(&log_queue, &sensor_msg, K_NO_WAIT);

            
        }

        k_msleep(THREAD_ANOMALY_DETECT_PERIOD_MS);
    }
}

// Perform simple anomaly detection by checking if value is out of valid range
            // if (reading.value < reading.min_value || reading.value > reading.max_value) 
            // {
            //     char alert[LOG_MSG_SIZE];
            //     snprintf(alert, sizeof(alert), "ALERT: %s - %s = %.2f out of range [%.2f-%.2f]",
            //         reading.machine_name, 
            //         reading.sensor_name, 
            //         (double)reading.value, 
            //         (double)reading.min_value, 
            //         (double)reading.max_value);    

            //     log_msg_t alert_msg = {.thread_id = 3};
            //     strncpy(alert_msg.message, alert, LOG_MSG_SIZE - 1);
            //     k_msgq_put(&log_queue, &alert_msg, K_NO_WAIT);
            // }