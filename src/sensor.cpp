/**
 * @file sensor.cpp
 * @brief Implementation of sensors, factory, and Machine logic.
 * 
 * Contains concrete sensor behavior and runtime composition of machines 
 * and their sensors.
*/

#include <memory>
#include <string>
#include <vector>
#include <stdio.h>
#include <iostream>

#include "sensor.h"
#include "sensor_wrapper.h"

// -----------------------------------------------------------------------------
// Concrete Sensor Implementations
// -----------------------------------------------------------------------------
void TempSensor::setValue(float value) { Value = value; }
float TempSensor::readValue() { return Value; }
std::string TempSensor::getType() { return "Temperature"; }

void PressureSensor::setValue(float value) { Value = value; }
float PressureSensor::readValue() { return Value; }
std::string PressureSensor::getType() { return "Pressure"; }

void VibrationSensor::setValue(float value) { Value = value; }
float VibrationSensor::readValue() { return Value; }
std::string VibrationSensor::getType() { return "Vibration"; }

// -----------------------------------------------------------------------------
// SensorFactory Implementation
// -----------------------------------------------------------------------------
/**
 * @brief Create a concrete sensor instance based on type identifier
 * 
 * Uses a simple string-based factory to construct the appropriate sensor
 * subclass at runtime.
 * 
 * @param type Sensor type identifier ("Temperature", "Pressure", "Vibration")
 * 
 * @return std::unique_ptr<Sensor> Owning pointer to the created sensor
 *         Return nullptr if the type is unsupported
*/
std::unique_ptr<Sensor> SensorFactory::createSensor(const std::string& type) 
{
    if (type == "Temperature") {
        return std::unique_ptr<Sensor>(new TempSensor());
    } else if (type == "Pressure") {
        return std::unique_ptr<Sensor>(new PressureSensor());
    } else if (type == "Vibration") {
        return std::unique_ptr<Sensor>(new VibrationSensor());
    }
    return nullptr;
}

// -----------------------------------------------------------------------------
// Machine Implementation
// -----------------------------------------------------------------------------
/**
 * @brief Construct a machine with a set of sensors
 * 
 * Sensors are created dynamically at runtime using the SensorFactory based on 
 * the provided sensor tpe list.
 * 
 * @param machineName Human-readable machine name
 * @param sensorTypes List of sensor type identifiers
 * @param machineType Machine classification
*/
Machine::Machine(const std::string& machineName, 
                 const std::vector<std::string>& sensorTypes, 
                 MachineType machineType) 
    : type(machineType), name(machineName)  
{
    for (const auto& type : sensorTypes) {
        auto sensor = SensorFactory::createSensor(type);
        if (sensor) {
            sensors.push_back(std::move(sensor));
        }
    }
}

/** @brief Display machine information and attached sensors */
void Machine::display() const {
    std::cout << "Machine: " << name << std::endl;
    for (const auto& sensor : sensors) {
        std::cout << "  - Sensor Type: " << sensor->getType() << std::endl;
    }
}

/**
 * @brief Set the value of a specific sensor by type
 * 
 * @param type Sensor type identifier
 * @param value Value to assign to the sensor
*/
void Machine::setSensorValue(const std::string& type, float value) {
    for (auto& sensor : sensors) {
        if (sensor->getType() == type) {
            sensor->setValue(value);
            return;
        }
    }
    std::cout << "Sensor type " << type 
              << " not found in machine " << name << std::endl;
}

/**
 * @brief Retrieve the current value of a specific sensor
 * 
 * @param type Sensor type identifier
 * @return Sensor value
 *         -1.0f if the sensor is not found
 */
float Machine::getSensorValue(const std::string& type) {
    for (const auto& sensor : sensors) {
        if (sensor->getType() == type) {
            return sensor->readValue();
        }
    }
    std::cout << "Sensor type " << type << " not found in machine " << name << std::endl;
    return -1.0f;
}