#include "sensor.h"
#include "sensor_wrapper.h"
#include <vector>
#include <string>
#include <cstring>

// Define sensor types for each machine type
const std::vector<std::string> air_compressor_sensors = {"Temperature", "Pressure", "Vibration"};
const std::vector<std::string> steam_boiler_sensors = {"Temperature", "Pressure"};
const std::vector<std::string> electric_motor_sensors = {"Temperature"};

MachineHandle create_machine(const char* machine_name, MachineType type) {
    std::vector<std::string> sensors;
    
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
    
    Machine* machine = new Machine(std::string(machine_name), sensors, type);
    return reinterpret_cast<MachineHandle>(machine);
}

void destroy_machine(MachineHandle handle) {
    Machine* machine = reinterpret_cast<Machine*>(handle);
    delete machine;
}

void describe_machine(MachineHandle handle) {
    Machine* machine = reinterpret_cast<Machine*>(handle);
    machine->display();
}

void set_sensor_value(MachineHandle handle, const char* sensor_type, float value) {
    Machine* machine = reinterpret_cast<Machine*>(handle);
    machine->setSensorValue(sensor_type, value);
}

float get_sensor_value(MachineHandle handle, const char* sensor_type) {
    Machine* machine = reinterpret_cast<Machine*>(handle);
    return machine->getSensorValue(sensor_type);
}

const char* get_machine_type_string(MachineType type) {
    switch (type) {
        case AIR_COMPRESSOR: return "Air Compressor";
        case STEAM_BOILER: return "Steam Boiler";
        case ELECTRIC_MOTOR: return "Electric Motor";
        default: return "Unknown Type";
    }
}

MachineType get_machine_type(MachineHandle handle) {
    Machine* machine = reinterpret_cast<Machine*>(handle);
    return machine->getType();
}