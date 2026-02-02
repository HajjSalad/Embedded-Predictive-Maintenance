
#include "demo.h"
#include "sensor_wrapper.h"
#include "tflite_wrapper.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/__assert.h>

// Thread to read the sensor data into the machines
void set_data(void) 
{
    while (1)
    {
        printf("\nSet machines values:\n");
        for (int i=0; i<NUM_MACHINES; i++)              // Set values for all the sensors in each machine
        {
            MachineType type = get_machine_type(machines[i]);
            if (type < 0 || type >= (sizeof(machine_configs) / sizeof(machine_configs[0]))) {
                printf("Error: Invalid machine type %d\n", type);
                continue;
            }

            const MachineConfig* config = &machine_configs[type];           // Use MachineType to index into machine_configs 

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
        }
        printf("\n");
        k_msleep(5000);  // Sleep for 3 seconds before printing again
    }
} 

// Thread to print sensor values of each machine
void print_data(void) 
{
    while (1) 
    {
        printf("\nGet machines values:\n");
        for (int i=0; i<NUM_MACHINES; i++)              
        {
            MachineType type = get_machine_type(machines[i]);
            if (type < 0 || type >= (sizeof(machine_configs) / sizeof(machine_configs[0]))) {
                printf("Error: Invalid machine type %d\n", type);
                continue;
            }

            const MachineConfig* config = &machine_configs[type];        // Use MachineType to index into machine_configs 

            printf("%s:", config->name);
            for (int s=0; s < config->num_sensors; s++) {
                const SensorConfig* sensor = &config->sensors[s];
                
                if (strlen(sensor->name) == 0) continue;                 // Skip invalid sensors

                float value = get_sensor_value(machines[i], sensor->name);
                if (s == 0) {
                    printf(" %s = %.2f\n",  
                        sensor->name, value);
                } else {
                    printf("                %s = %.2f\n",  
                        sensor->name, value);
                }
            }
        }
        printf("\n");
        k_msleep(5000);  // Sleep for 5 seconds before printing again
    }
}

// Start the threads
K_THREAD_DEFINE(blink0_id, STACKSIZE, blink0, NULL, NULL, NULL, PRIORITY, 0, 0);                    // Confirm the program is alive
K_THREAD_DEFINE(set_data_id, STACKSIZE, set_data, NULL, NULL, NULL, PRIORITY + 1, 0, 0);            // Read the sensor data into the machines
K_THREAD_DEFINE(print_data_id, STACKSIZE, print_data, NULL, NULL, NULL, PRIORITY + 2, 0, 0);        // Print the machine sensor values

