/**
 * @file thread_sensor_write.c
 * @brief
 * 
 * Description
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zephyr/kernel.h>

#include "threads.h"
#include "wrapper.h"
#include "shared_resources.h"
#include "circular_buffer.h"

/**
 * @brief Thread 1: Write simulated sensor values into sensor objects
 * 
 * Iterates through all machines and their sensors, generates a random value
 * within each sensor's configured range, and writes it via the C++ wrapper.
 * 
 * All output is routed through the logging message queue to be printed
 * by system_logger (Thread 5).
 * 
 * @note Runs every 5000ms
*/
void sensor_write(void) 
{
    while (1) 
    {
        log_msg_t msg = {.thread_id = 1, .message = "Setting sensor values:"};
        k_msgq_put(&log_queue, &msg, K_NO_WAIT);

        // Iterate through each machine and set all sensor values
        for (u_int8_t i=0U; i<NUM_MACHINES; i++) 
        {
            // Get machine handle
            MachineHandle machine = get_machine(i);
            if (machine == NULL) {
                continue;           // Skip invalid machine
            }
            
            // Get machine type, name and sensor count
            MachineType machineType = get_machine_type(machine);
            uint8_t numSensors      = get_sensor_count(machine);
            const char* machineName = get_machine_name(machine);

            // Set values for each sensor in this machine
            for (uint8_t s = 0U; s < numSensors; s++) 
            {
                // Get sensor type and range
                const char* sensorType = get_sensor_type(machine, s);
                float minVal           = get_sensor_min_value(machine, sensorType);
                float maxVal           = get_sensor_max_value(machine, sensorType);

                // Generate random value within range
                float range = maxVal - minVal;
                float value = minVal + ((float)rand() / (float)RAND_MAX) * range;

                // Acquire mutex & Set the sensor value
                (void)k_mutex_lock(&sensor_mutex, K_FOREVER);
                set_sensor_value(machine, sensorType, value);
                (void)k_mutex_unlock(&sensor_mutex)

                // Log the operation
                static char buf[LOG_MSG_SIZE];
                snprintf(buf, sizeof(buf), "  %-15s | %-12s = %6.2f [%.2f-%.2f]",
                    machineName, sensorType,
                    (double)value,
                    (double)minVal,
                    (double)maxVal);

                log_msg_t sensor_msg = {.thread_id = 1};
                strncpy(sensor_msg.message, buf, LOG_MSG_SIZE - 1);
                (void)k_msgq_put(&log_queue, &sensor_msg, K_NO_WAIT);
            }
        }
        // Sleep before next sensor update cycle
        k_msleep(THREAD_SENSOR_WRITE_PERIOD_MS);
    }
}