/**
 * @file machines.c
 * @brief Machine creation and management.
 * 
 * This file implements functions to create and manage industrial machine
 * instances and their associated sensors.
*/

#include <stdio.h>

#include "machines.h"
#include "sensor_wrapper.h"

/** @brief Array to hold machine handles */
MachineHandle machines[NUM_MACHINES];

/**
 * @brief Generate machine instances and register their sensors
 * 
 * Uses the `create_machine` function from the C++ wrapper to create machines
 * for Air Compressor, Steam Boiler, and Electric Motor. 
 * 
 * Machine handles are stored in the global `machines` array. 
 * 
 * After creation, each machine is described via `describe_machine`.
*/
void generate_machines_and_sensors() 
{
    //   To create and add machine:
    //   1. Increment NUM_MACHINES
    //   2. Create machine instance   

    // Create a Machine instance with specified sensors
    machines[0] = create_machine("Air_Compressor_1", AIR_COMPRESSOR);
    machines[1] = create_machine("Steam_Boiler_1", STEAM_BOILER);
    machines[2] = create_machine("Electric_Motor_1", ELECTRIC_MOTOR);

    // Print Machine details: name and sensors
    for (int i=0; i<NUM_MACHINES; i++) {
        describe_machine(machines[i]);
    }
}

/**
 * @brief Cleanup all machine instances
 * 
 * Calls `destroy_machine` for each machine to free memory allocated in C++.
*/
void cleanup_machines(void)
{
    for (int i = 0; i < NUM_MACHINES; i++) {
        destroy_machine(machines[i]);
        machines[i] = NULL;
    }
}
