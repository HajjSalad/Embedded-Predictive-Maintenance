#ifndef MACHINES_H
#define MACHINES_H

#include "sensor_wrapper.h"

/**
 * @file machines.h
 * @brief Defines machine configuration and interface to generate machine instances.
*/

/** @brief Number of machines currently in the system */
#define NUM_MACHINES 3

/** @brief Array to hold machine handles */
extern MachineHandle machines[NUM_MACHINES];

// Function Prototype
void generate_machines_and_sensors(void);

/** Machine configuration for each machine type */
static const MachineConfig machine_configs[] = 
{
    {   "Air Compressor",
        {   {"Temperature", 60.0f, 100.0f},         // 60 - 100°C
            {"Pressure", 72.0f, 145.0f},            // 72 - 145 psi
            {"Vibration", 0.5f, 2.0f} },            // 0.5 - 2.0 mm/s
        3                                           // Number of sensors
    },
    {   "Steam Boiler",
        {   {"Temperature", 150.0f, 250.0f},        // 150 - 250°C
            {"Pressure", 87.0f, 360.0f},            // 87 - 360 psi
            {"", 0.0f, 0.0f} },                     // Leave these empty
        2                                           // Number of sensors
    },
    {   "Electric Motor",
        {   {"Temperature", 60.0f, 105.0f},         // 60 - 105°C
            {"", 0.0f, 0.0f},                       // Leave empty
            {"", 0.0f, 0.0f} },                     // Leave empty
        1                                           // Number of sensors
    }
};

#endif // MACHINES_H