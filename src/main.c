/**
 * @file  main.c
 * @brief System entry point and initialization sequence.
 * 
 * This file is responsoble for:
 *  - Bringing up core system components 
 *  - Initializing the application modules
 *  - Spawning application threads after initialization
*/
#include <zephyr/kernel.h>

#include "shared_resources.h"
#include "demo.h"
#include "threads.h"
#include "wrapper.h"
#include "circular_buffer.h"

/** @brief Stack size in bytes allocated for each thread */
#define STACK_SIZE      2048U

/** @brief Thread priorities - In Zephyr, 0 is highest priority */
#define PRIORITY_3      3
#define PRIORITY_4      4
#define PRIORITY_5      5
#define PRIORITY_6      6
#define PRIORITY_7      7

/** @brief Instantiate the circular buffer */
CircularBuffer circular_buffer;  

/**
 * @brief Message queue for passing log messages from threads 1-4 to system_logger
 * 
 * Holds up to @ref LOG_QUEUE_SIZE messages of type @ref log_msg_t.
 * Defined here and declared extern in shared_resources.h.
*/
K_MSGQ_DEFINE(log_queue, sizeof(log_msg_t), LOG_QUEUE_SIZE, MESSAGE_ALIGN);

/* */
K_MUTEX_DEFINE(sensor_mutex);
K_MUTEX_DEFINE(buffer_mutex);

// /** @brief Thread stacks - statically allocated */
K_THREAD_STACK_DEFINE(sensor_write_stack,    STACK_SIZE);
K_THREAD_STACK_DEFINE(sensor_read_stack,     STACK_SIZE);
K_THREAD_STACK_DEFINE(anomaly_detect_stack,  STACK_SIZE);
K_THREAD_STACK_DEFINE(anomaly_handle_stack,  STACK_SIZE);
K_THREAD_STACK_DEFINE(system_log_stack,      STACK_SIZE);

// /** @brief Declare Thread control blocks (TCB holds: priority, stack pointers etc) */
struct k_thread sensor_write_thread;
struct k_thread sensor_read_thread;
struct k_thread anomaly_detect_thread;
struct k_thread anomaly_handle_thread;
struct k_thread system_log_thread;

/**
 * @brief Spawn all application threads.
 *
 * Called at the end of main() after all initialization is complete,
 * guaranteeing that machines, sensors, and the circular buffer are
 * ready before any thread begins execution.
*/
static void spawn_threads(void)
{
    /**
     * k_thread_create() creates and starts the thread: Initializes the TCB,
     * links it to the stack, and adds it to the scheduler's ready queue.
     */
    k_thread_create(&sensor_write_thread, sensor_write_stack,
                    K_THREAD_STACK_SIZEOF(sensor_write_stack),
                    (k_thread_entry_t)sensor_write,
                    NULL, NULL, NULL, PRIORITY_3, 0, K_NO_WAIT);
    //printk("sensor_write thread created\n");

    k_thread_create(&sensor_read_thread, sensor_read_stack,
                    K_THREAD_STACK_SIZEOF(sensor_read_stack),
                    (k_thread_entry_t)sensor_read,
                    NULL, NULL, NULL, PRIORITY_4, 0, K_NO_WAIT);
    //printk("sensor_read thread created\n");

    k_thread_create(&anomaly_detect_thread, anomaly_detect_stack,
                    K_THREAD_STACK_SIZEOF(anomaly_detect_stack),
                    (k_thread_entry_t)anomaly_detect,
                    NULL, NULL, NULL, PRIORITY_5, 0, K_NO_WAIT);
    //printk("anomaly_detect thread created\n");

    k_thread_create(&anomaly_handle_thread, anomaly_handle_stack,
                    K_THREAD_STACK_SIZEOF(anomaly_handle_stack),
                    (k_thread_entry_t)anomaly_handle,
                    NULL, NULL, NULL, PRIORITY_6, 0, K_NO_WAIT);
    //printk("anomaly_handle thread created\n");

    k_thread_create(&system_log_thread, system_log_stack,
                    K_THREAD_STACK_SIZEOF(system_log_stack),
                    (k_thread_entry_t)system_log,
                    NULL, NULL, NULL, PRIORITY_7, 0, K_NO_WAIT);
    //printk("system_log thread created\n");
}

/**
 * @brief Main system entry point.
 * 
 * Performs one-time system initialization:
 *  - Run C++ interopability demo
 *  - Creates machine instances and registers sensors
 *  - Initializes the circular buffer
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

    // C++ interopability Demo                                     
    printk("Demo Message: %s\n", demo_get_message());

    // Create machines and register their sensors
    generate_machines_and_sensors();

    // Initialize the circular buffer
    circular_buffer_init(&circular_buffer);

    // Spawn threads after initialization is complete
    spawn_threads();

    //printk("Threads spawned\n");
    
    // Keep main alive - threads drive all application logic
    while (1) {
        k_sleep(K_FOREVER);
    }

    return 0;
}
