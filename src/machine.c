
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


#define NUM_MACHINES         3



MachineHandle machines[NUM_MACHINES];

static const MachineConfig machine_configs[] = 
{
    {   "Air Compressor",
        {   {"Temperature", 60.0f, 100.0f},         // 60 - 100°C
            {"Pressure", 72.0f, 145.0f},            // 72 - 145 psi
            {"Vibration", 0.5f, 2.0f} },            // 0.5 - 2.0 mm/s
        3
    },
    {   "Steam Boiler",
        {   {"Temperature", 150.0f, 250.0f},        // 150 - 250°C
            {"Pressure", 87.0f, 360.0f},            // 87 - 360 psi
            {"", 0.0f, 0.0f} },                     // Leave these empty
        2
    },
    {   "Electric Motor",
        {   {"Temperature", 60.0f, 105.0f},         // 60 - 105°C
            {"", 0.0f, 0.0f},                       // Leave empty
            {"", 0.0f, 0.0f} },                     // Leave empty
        1
    }
};

void generate_machines() 
{
    //   To create and add machine:
    //   1. Increment NUM_MACHINES
    //   2. Create machine instance   

    // Create a Machine instance with specified sensors
    machines[0] = create_machine("Air_Compressor_1", AIR_COMPRESSOR);
    machines[1] = create_machine("Steam_Boiler_1", STEAM_BOILER);
    machines[2] = create_machine("Electric_Motor_1", ELECTRIC_MOTOR);

    // Print Machine details - name, sensors
    for (int i=0; i<NUM_MACHINES; i++) {
        describe_machine(machines[i]);
    }
}