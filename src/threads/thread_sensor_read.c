/**
 * @file thread_sensor_read.c
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
 * @brief Thread 2: Read sensor values and write into the circular buffer
 * 
 * Iterates through all machines and their sensors, retrieves the current
 * sensor value via the C++ wrapper, and pushes a @ref sensor_reading into
 * the circular buffer for consumption by the anomaly_detector (Thread 3).
 * 
 * All output is routed through the logging message queue to be printed
 * by system_logger (Thread 5).
 * 
 * @note Runs every 5000ms
*/
void sensor_read(void)
{
    while (1) 
    {
        log_msg_t msg = {.thread_id = 1, .message = "Getting sensor values:"};
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

                // Acquire mutex & Get the sensor value
                (void)k_mutex_lock(&sensor_mutex, K_FOREVER);
                float value = get_sensor_value(machine, sensorType);
                (void)k_mutex_unlock(&sensor_mutex);

                // Format the sensor readings for circular buffer 
                struct sensor_reading reading = {
                    .machine_name = machineName,
                    .sensor_type = sensorType,
                    .value = value,
                    .min_value = minVal,
                    .max_value = maxVal
                };
                strncpy(reading.machine_name, machineName, sizeof(reading.machine_name) - 1);
                strncpy(reading.sensor_name, sensorType, sizeof(reading.sensor_name) - 1);

                // Acquire mutex and write to circular buffer
                (void)k_mutex_lock(&buffer_mutex, K_FOREVER);
                cb_write(&circular_buffer, &reading);
                (void)k_mutex_unlock(&buffer_mutex);

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
        k_msleep(THREAD_SENSOR_READ_PERIOD_MS);
    }
}