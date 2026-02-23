/**
 * @file sensor.cpp
 * @brief Implementation of sensors, factory, and Machine logic.
 * 
 * Contains concrete sensor behavior and runtime composition of machines 
 * and their sensors.
*/

#include <string.h>
#include <stdint.h>

#include "sensor.h"
#include "wrapper.h"

// Static sensor pool (Object pool pattern)
static TempSensor       tempPool[MAX_TEMP_SENSORS];
static PressureSensor   pressPool[MAX_PRESS_SENSORS];
static VibrationSensor  vibPool[MAX_VIB_SENSORS];

// Pool allocation indices
static uint8_t tempIndex  = 0U;
static uint8_t pressIndex = 0U;
static uint8_t vibIndex   = 0U;

// Static Machine Pool
static Machine machinePool[NUM_MACHINES] = {
    Machine("Station_A_Air_Compressor", AIR_COMPRESSOR),
    Machine("Station_A_Steam_Boiler", STEAM_BOILER),
    Machine("Station_A_Electric_Motor", ELECTRIC_MOTOR)
};

// Allocate a sensor from the appropriate static pool
Sensor* SensorFactory::createSensor(const char* type, uint16_t sensorNumber, 
                                    float minVal, float maxVal)
{
    if (type == nullptr) {
        return nullptr;
    }

    // Temperature sensor pool
    if (strcmp(type, "Temperature") == 0 && tempIndex < MAX_TEMP_SENSORS) {
        // Construct sensor in-place at next available slot
        tempPool[tempIndex] = TempSensor(sensorNumber, minVal, maxVal);
        return &tempPool[tempIndex++];
    }

    // Pressure sensor pool
    else if (strcmp(type, "Pressure") == 0 && pressIndex < MAX_PRESS_SENSORS) {
        pressPool[pressIndex] = PressureSensor(sensorNumber, minVal, maxVal);
        return &pressPool[pressIndex++];
    }

    // Vibration sensor pool
    else if (strcmp(type, "Vibration") == 0 && vibIndex < MAX_VIB_SENSORS) {
        vibPool[vibIndex] = VibrationSensor(sensorNumber, minVal, maxVal);
        return &vibPool[vibIndex++];
    }
    
    return nullptr;     // pool exhausted or unknown type
}

Machine::Machine(const char* machineName, MachineType machineType)
    : name(machineName), type(machineType), sensorCount(0U)
{
    // Initialize sensor array to nullptr
    for (uint8_t i=0U; i < MAX_SENSORS; i++){
        sensors[i] = nullptr;
    }
}

void Machine::addSensor(Sensor* sensor)
{
    if (sensor == nullptr || sensorCount >= MAX_SENSORS){
        return;     // Guard against null or full
    }

    sensors[sensorCount++] = sensor;
}

void Machine::setSensorValue(const char* sensorType, float value)
{
    if (sensorType == nullptr) {
        return;     
    }

    for (uint8_t i=0U; i < sensorCount; i++) {
        if (sensors[i] != nullptr && strcmp(sensors[i]->getType(), sensorType) == 0) {
            sensors[i]->setValue(value);
            return;
        }
    }
}

float Machine::getSensorValue(const char* sensorType) const
{
    if (sensorType == nullptr) {
        return 0.0f;     
    }

    for (uint8_t i=0U; i < sensorCount; i++) {
        if (sensors[i] != nullptr && strcmp(sensors[i]->getType(), sensorType) == 0) {
            return sensors[i]->readValue();
        }
    }
    return 0.0f;    // Sensor not found
}

float Machine::getSensorMinValue(const char* sensorType) const
{
    if (sensorType == nullptr) {
        return 0.0f;     
    }

    for (uint8_t i=0U; i < sensorCount; i++) {
        if (sensors[i] != nullptr && strcmp(sensors[i]->getType(), sensorType) == 0) {
            return sensors[i]->getMinValue();
        }
    }
    return 0.0f;    // Sensor not found
}

float Machine::getSensorMaxValue(const char* sensorType) const
{
    if (sensorType == nullptr) {
        return 0.0f;     
    }

    for (uint8_t i=0U; i < sensorCount; i++) {
        if (sensors[i] != nullptr && strcmp(sensors[i]->getType(), sensorType) == 0) {
            return sensors[i]->getMaxValue();
        }
    }
    return 0.0f;    // Sensor not found
}

const char* Machine::getSensorType(u_int8_t sensorIndex) const
{
    if (sensorIndex >= sensorCount || sensors[sensorIndex] == nullptr) {
        return "unknown";
    }
    return sensors[sensorIndex]->getType();
}