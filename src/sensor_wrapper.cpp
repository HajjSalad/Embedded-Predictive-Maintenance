/**
 * @file sensor_wrapper.cpp
 * @brief C-compatible warpper for C++ Machine and Sensor classes.
 * 
 * This file provides a C API that allows pure C code (including Zephyr
 * threads written in C) to interact with C++ Machine and Sensor objects.
 * 
 *  
*/

#include <vector>
#include <string>
#include <cstring>

#include "sensor.h"
#include "sensor_wrapper.h"

// --------------------------------------------------------------------------
// Sensor configuration per machine type
// -------------------------------------------------------------------------- 

/**
 * @brief Sensor set for an air compressor
 * 
 * Air compressors are monitored for: Temperature, Pressure and Vibration
*/
const std::vector<std::string> air_compressor_sensors = {
    "Temperature", "Pressure", "Vibration"
};

/**
 * @brief Sensor set for an Steam Boiler
 * 
 * Steam boilers are monitored for: Temperaature and Pressure
*/
const std::vector<std::string> steam_boiler_sensors = {
    "Temperature", "Pressure"
};

/**
 * @brief Sensor set fro Electric Motor
 * 
 * Electric motors are monitored for: Temperature
*/
const std::vector<std::string> electric_motor_sensors = {
    "Temperature"
};

// --------------------------------------------------------------------------
//  C API implementation
// --------------------------------------------------------------------------

/**
 * @brief Create a machine instance with sensors based on machine type.
 * 
 * This function dynamically allocates a C++ Machine object and returns 
 * it as an opaque handles usable from C code.
 * 
 * Sensor composition is determined at runtime based on the provided 
 * machine type
 * 
 * @param machine_name Human-readable machine name
 * @param type         Machine type enum
 * 
 * @return Opaque handle to the created machine
*/
MachineHandle create_machine(const char* machine_name, MachineType type) 
{
    std::vector<std::string> sensors;
    
    // Select sensor configuration based on machine type
    switch(type) {
        case AIR_COMPRESSOR:
            sensors = air_compressor_sensors;
            break;
        case STEAM_BOILER:
            sensors = steam_boiler_sensors;
            break;
        case ELECTRIC_MOTOR:
            sensors = electric_motor_sensors;
            break;
    }
    
    // Allocate C++ Machine object
    Machine* machine = new Machine(std::string(machine_name), sensors, type);

    // Return as opaque C handle
    return reinterpret_cast<MachineHandle>(machine);
}

/**
 * @brief Destroy a machine instance and free its resources.
 * 
 * @param handle Opaque nachine handle
*/
void destroy_machine(MachineHandle handle) 
{
    Machine* machine = reinterpret_cast<Machine*>(handle);
    delete machine;
}

/**
 * @brief Print a textual description of the machine and its sensors
 * 
 * @param handle Opaque machine handle
*/
void describe_machine(MachineHandle handle) 
{
    Machine* machine = reinterpret_cast<Machine*>(handle);
    machine->display();
}

/**
 * @brief Set a sensor value on a machine.
 * 
 * @param handle      Opaque machine handle
 * @param sensor_type Sensor type string (e.g. "Temperature")
 * @param value       New sensor reading
*/
void set_sensor_value(MachineHandle handle, const char* sensor_type, float value) 
{
    Machine* machine = reinterpret_cast<Machine*>(handle);
    machine->setSensorValue(sensor_type, value);
}

/**
 * @brief Retrieve a sensor value from a machine.
 *
 * @param handle      Opaque machine handle
 * @param sensor_type Sensor type string
 * @return            Sensor reading, or negative value if not found
*/
float get_sensor_value(MachineHandle handle, const char* sensor_type) 
{
    Machine* machine = reinterpret_cast<Machine*>(handle);
    return machine->getSensorValue(sensor_type);
}

/**
 * @brief Convert a machine type enum to a human-readable string.
 *
 * @param type Machine type enum
 * @return String representation of the machine type
*/
const char* get_machine_type_string(MachineType type) 
{
    switch (type) {
        case AIR_COMPRESSOR: return "Air Compressor";
        case STEAM_BOILER: return "Steam Boiler";
        case ELECTRIC_MOTOR: return "Electric Motor";
        default: return "Unknown Type";
    }
}

/**
 * @brief Get the machine type of a given machine instance.
 *
 * @param handle Opaque machine handle
 * @return MachineType enum
*/
MachineType get_machine_type(MachineHandle handle) {
    Machine* machine = reinterpret_cast<Machine*>(handle);
    return machine->getType();
}