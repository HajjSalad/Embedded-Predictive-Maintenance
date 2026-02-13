/**
 * @file main.c
 * @brief System entry point and initialization sequence.
 * 
 * This file is responsoble for:
 *  - Bringing up core system components 
 *  - Initializing the application modules
 *  - Spawning application threads after initialization
*/

#include "demo.h"
#include "threads.h"
#include "machines.h"

#include <zephyr/kernel.h>

/** @brief Stack size of the threads */
#define STACK_SIZE      1024

/** @brief Thread priorities */
#define PRIORITY_1      3
#define PRIORITY_2      4
#define PRIORITY_3      5
#define PRIORITY_4      6
#define PRIORITY_5      7

/** @brief Thread stacks */
K_THREAD_STACK_DEFINE(sensor_writer_stack,  STACK_SIZE);
K_THREAD_STACK_DEFINE(data_collector_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(anomaly_detector_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(anomaly_handler_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(system_logger_stack,  STACK_SIZE);

/** @brief Thread control blocks */
struct k_thread sensor_writer_thread;
struct k_thread data_collector_thread;
struct k_thread anomaly_detector_thread;
struct k_thread anomaly_handler_thread;
struct k_thread system_logger_thread;

/**
 * @brief Main system entry point.
 * 
 * Performs one-time system initialization:
 *  - Run C++ demo
 *  - Creates machine instances and registers sensors
 *  - Spawns application threads
 *
 * After initialization, the function idles while
 * background threads execute the application logic.
 *
 * @return Never returns
*/
int main(void) 
{
    printk("\n*** Program Start ***\n");                 

    // C++ Demo                                     
    printk("Demo Message: %s\n", demo_get_message());

    // Create machines and register sensors
    generate_machines_and_sensors();

    // Spawn threads after initialization is complete
    k_thread_create(&sensor_writer_thread, sensor_writer_stack,
                    K_THREAD_STACK_SIZEOF(sensor_writer_stack),
                    (k_thread_entry_t)sensor_data_writer,
                    NULL, NULL, NULL, PRIORITY_1, 0, K_NO_WAIT);

    k_thread_create(&data_collector_thread, data_collector_stack,
                    K_THREAD_STACK_SIZEOF(data_collector_stack),
                    (k_thread_entry_t)data_collector,
                    NULL, NULL, NULL, PRIORITY_2, 0, K_NO_WAIT);

    k_thread_create(&anomaly_detector_thread, anomaly_detector_stack,
                    K_THREAD_STACK_SIZEOF(anomaly_detector_stack),
                    (k_thread_entry_t)anomaly_detector,
                    NULL, NULL, NULL, PRIORITY_3, 0, K_NO_WAIT);

    k_thread_create(&anomaly_handler_thread, anomaly_handler_stack,
                    K_THREAD_STACK_SIZEOF(anomaly_handler_stack),
                    (k_thread_entry_t)anomaly_handler,
                    NULL, NULL, NULL, PRIORITY_4, 0, K_NO_WAIT);

    k_thread_create(&system_logger_thread, system_logger_stack,
                    K_THREAD_STACK_SIZEOF(system_logger_stack),
                    (k_thread_entry_t)system_logger,
                    NULL, NULL, NULL, PRIORITY_5, 0, K_NO_WAIT);

    // Keep main alive
    while (1) {
        k_sleep(K_FOREVER);
    }

    return 0;
}
