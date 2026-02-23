/**
 * @file sensor_wrapper.cpp
 * @brief C-compatible warpper for C++ Machine and Sensor classes.
 * 
 * This file provides a C API that allows pure C code (including Zephyr
 * threads written in C) to interact with C++ Machine and Sensor objects.
*/

#include "wrapper.h"
#include "sensor.h"

extern Machine machinePool[NUM_MACHINES];

extern "C" void generate_machines_and_sensors(void) 
{
    // Air Compressor: Temperature (60-100°C), Pressure (72-145 psi), Vibration (0.5-2.0 mm/s)
    machinePool[0].addSensor(SensorFactory::createSensor("Temperature", 0,  60,  100));
    machinePool[0].addSensor(SensorFactory::createSensor("Pressure",    1,  72,  145));
    machinePool[0].addSensor(SensorFactory::createSensor("Vibration",   2, 0.5,  2.0));

    // Steam Boiler: Temperature (150-250°C), Pressure (87-360 psi)
    machinePool[1].addSensor(SensorFactory::createSensor("Temperature", 3, 150,  250));
    machinePool[1].addSensor(SensorFactory::createSensor("Pressure",    4,  87,  360));

    // Electric Motor: Temperature (60-105°C)
    machinePool[2].addSensor(SensorFactory::createSensor("Temperature", 5,  60,  105));
}

extern "C" MachineHandle get_machine(uint8_t index) 
{
    if (index >= NUM_MACHINES) {
        return nullptr;
    }
    return reinterpret_cast<MachineHandle>(&machinePool[index]);
}

extern "C" const char* get_machine_name(MachineHandle machine)
{
    if (machine == nullptr) {
        return "unknown";
    }
    Machine* m = reinterpret_cast<Machine*>(machine);
    return m->getName();
}

extern "C" MachineType* get_machine_type(MachineHandle machine)
{
    if (machine == nullptr) {
        return;
    }
    Machine* m = reinterpret_cast<Machine*>(machine);
    m->getType();
}

extern "C" void set_sensor_value(MachineHandle machine, const char* sensorType, float value)
{
    if (machine == nullptr) {
        return;
    }
    Machine* m = reinterpret_cast<Machine*>(machine);
    m->setSensorValue(sensorType, value);
}

extern "C" float get_sensor_value(MachineHandle machine, const char* sensorType) 
{
    if (machine == nullptr) {
        return 0.0f;
    }
    Machine* m = reinterpret_cast<Machine*>(machine);
    return m->getSensorValue(sensorType);
}

extern "C" float get_sensor_min_value(MachineHandle machine, const char* sensorType) 
{
    if (machine == nullptr) {
        return 0.0f;
    }
    Machine* m = reinterpret_cast<Machine*>(machine);
    return m->getSensorMinValue(sensorType);
}

extern "C" float get_sensor_max_value(MachineHandle machine, const char* sensorType) 
{
    if (machine == nullptr) {
        return 0.0f;
    }
    Machine* m = reinterpret_cast<Machine*>(machine);
    return m->getSensorMaxValue(sensorType);
}

extern "C" uint8_t get_sensor_count(MachineHandle machine)
{
    if (machine == nullptr) {
        return 0U;
    }
    Machine* m = reinterpret_cast<Machine*>(machine);
    return m->getSensorCount();
}

extern "C" const char* get_sensor_type(MachineHandle machine, uint8_t sensorIndex) 
{
    if (machine == nullptr) {
        return "Unknown";
    }
    Machine* m = reinterpret_cast<Machine*>(machine);
    return m->getSensorType(sensorIndex);
}
