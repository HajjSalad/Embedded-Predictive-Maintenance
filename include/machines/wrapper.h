#ifndef WRAPPER_H
#define WRAPPER_H

/**
 * @file sensor_wrapper.h
 * @brief C-compatible wrapper interface for Machine and Sensor system.
*/

#include <stdint.h>

#define NUM_MACHINES        3U

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @enum MachineType
 * @brief Enumeration of supported machine types.
 * 
 * Used to classify machines at creation time.
*/
typedef enum {
    AIR_COMPRESSOR,
    STEAM_BOILER,
    ELECTRIC_MOTOR
} MachineType;

// Opaque handle for Machine objects
typedef void* MachineHandle;

// Initialization
void generate_machines_and_sensors(void);

// Machine access
MachineHandle get_machine(uint8_t index);
const char* get_machine_name(MachineHandle machine);
MachineType* get_machine_type(MachineHandle machine);
uint8_t get_sensor_count(MachineHandle machine);

// Sensor Operations
const char* get_sensor_type(MachineHandle machine, uint8_t sensorIndex);
void set_sensor_value(MachineHandle machine, const char* sensorType, float value);
float get_sensor_value(MachineHandle machine, const char* sensorType);
float get_sensor_min_value(MachineHandle machine, const char* sensorType);
float get_sensor_max_value(MachineHandle machine, const char* sensorType);

#ifdef __cplusplus
}
#endif

#endif // WRAPPER_H