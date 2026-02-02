/**
 * @file main.c
 * @brief System entry point and initialization sequence.
 * 
 * This file is responsoble for:
 *  - Bringing up core system components 
 *  - Initializing the application modules
*/

#include "demo.h"

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

/**
 * @brief Main system entry point.
 * 
 * Performs one-time system initialization:
 *  - Run C++ demo
 *  - Creates machine instances
 *  - Initializes TensorFlow Lite runtime
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
    //generate_machines_and_sensors();

    // Start threads
    //start_threads();

    // Keep main alive
    while (1) {
        k_sleep(K_FOREVER);
    }

    return 0;
}
