/**
 * @file threads.c
 * @brief
 * 
 * Description
*/

#include <stdlib.h>
#include <zephyr/kernel.h>

#include "threads.h"
#include "machines.h"

/**
 * @brief Message queue for passing log messages from threads 1-4 to system_logger
 * 
 * Holds up to @ref LOG_QUEUE_SIZE messages of type @ref log_msg_t.
 * Defined here and declared extern in threads.h.
*/
K_MSGQ_DEFINE(log_queue, sizeof(log_msg_t), LOG_QUEUE_SIZE, MESSAGE_ALIGN);

/**
 * @brief Thread 1: Write simulated sensor values into sensor objects
 * 
 * 
*/
void sensor_data_writer(void) 
{
    while (1) {
        log_msg_t msg = {.thread_id = 1, .message = "Set machines values:"};
        k_msgq_put(&log_queue, &msg, K_NO_WAIT);

        // Iterate through each machine and set all sensor values in that machine
        for (int i=0; i<NUM_MACHINES; i++) 
        {
            MachineType type = get_machine_type(machines[i]);
            if (type < 0 || type >= (sizeof(machine_configs) / sizeof(machine_configs[0]))) {
                printf("Error: Invalid machine type %d\n", type); -> how can we handle this better? 
                continue;
            }
        }

        // Use MachineType to index into machine_configs 
        const MachineConfig* config = &machine_configs[type];

        printf("%s:", config->name);
        for (int s=0; s < config->num_sensors; s++) {
            const SensorConfig* sensor = &config->sensors[s];
            
            if (strlen(sensor->name) == 0) continue;                    // Skip invalid sensors

            float range = sensor->max_value - sensor->min_value;
            float value = sensor->min_value + (rand() / (float)RAND_MAX) * range;

            set_sensor_value(machines[i], sensor->name, value);
            if (s == 0) {
                printf(" %s = %.2f  [range %.1f-%.1f]\n",  
                    sensor->name, (double)value, (double)sensor->min_value, (double)sensor->max_value);
            } else {
                printf("                %s = %.2f  [range %.1f-%.1f]\n",  
                    sensor->name, (double)value, (double)sensor->min_value, (double)sensor->max_value);
            }
        }
        k_msleep(2000);
    }
}

/**
 * @brief Thread 2: Read sensor values and write into the circular buffer
 * 
 * 
*/
void data_collector(void)
{
    while (1) {
        log_msg_t msg = {.thread_id = 2, .message = "In thread 2"};
        k_msgq_put(&log_queue, &msg, K_NO_WAIT);
        k_msleep(500);
    }
}

/**
 * @brief Thread 3: Consume data from the circular buffer and perform anomaly detection
 * 
 * 
*/
void anomaly_detector(void)
{
    while (1) {
        log_msg_t msg = {.thread_id = 3, .message = "In thread 3"};
        k_msgq_put(&log_queue, &msg, K_NO_WAIT);
        k_msleep(500);
    }
}

/**
 * @brief Thread 4: Handle detected anomalies (event-driven -> triggered by anomaly_detector)
 * 
 * 
 * 
*/
void anomaly_handler(void) 
{
    while (1) {
        log_msg_t msg = {.thread_id = 4, .message = "In thread 4"};
        k_msgq_put(&log_queue, &msg, K_NO_WAIT);
        k_msleep(500);
    }
}

/**
 * @brief Thread 5: Consume log messages from the logging queue and print to terminal
 * 
 * 
 * 
*/
void system_logger(void) 
{
    log_msg_t msg;

    while (1) {
        k_msgq_get(&log_queue, &msg, K_FOREVER);
        printk("Thread %d: %s\n", msg.thread_id, msg.message);
    }
}
