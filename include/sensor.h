#ifndef SENSOR_H
#define SENSOR_H

/**
 * @file sensor.h
 * @brief Sensor abstractions, concrete sensor types, and Machine model.
 * 
 * This file defines:
 *  - A polymorphic Sensor interface
 *  - Concrete sensor implementation (Temperature, Pressure, Vibration)
 *  - A SensorFactory for controlled sensor creation
 *  - A Machine class that aggregates sensors
*/

#include <memory>
#include <vector>
#include <string>
#include <stdint.h>

#include "sensor_wrapper.h"

/**
 * @class Sensor
 * @brief Abstract base class for all sensors
 * 
 * Defines the common interface that all sensor types must implement. This
 * allows machines to interact with sensors polymorphically without knowing
 * their concrete type.
 * 
 * Provides a protected data member `Value` so all derived sensors follow 
 * a consistent pattern for storing their latest reading.
 * 
 * The public methods are declared pure virtual, enforcing that every concrete 
 * sensor provides its own implementation and enabling runtime polymorphism 
 * when accessed via base-class pointers.
*/
class Sensor {
protected:
    float Value;        /**< Latest sensor reading */

public:
    /** 
    * @brief Set the sensor value.
    * @param value New sensor reading 
    */
    virtual void setValue(float value) = 0;

    /** 
    * @brief Read the current sensor value
    * @return Current sensor reading 
    */
    virtual float readValue() = 0;

    /**
    * @brief Get the sensor type as a string.
    * @return Sensor type identifier
    */
    virtual std::string getType() = 0;

    /** @brief Virtual deconstructor for safe polymorphic cleanup */
    virtual ~Sensor() {}
};

/**
 * @class TempSensor
 * @brief Concrete sensor representing a temperature sensor.
 * 
 * Inherits from the abstract base class 'Sensor' and implements all
 * its pure virtual methods. 
 * 
 * Stores the latest temperature reading in the inherited `Value` member.
 * 
 * This class can be used polymorphically through `Sensor*` or 
 * `std::unique_ptr<Sensor>` without knowing the concrete type.
*/
class TempSensor : public Sensor {
public:
    /**
    * @brief Set the temperature sensor reading
    * @param value The new temperature value to store
    */
    void setValue(float value) override;

    /**
    * @brief Read the current temperature value
    * @return The latest temperature reading
    */
    float readValue() override;

    /**
    * @brief Get the sensor type identifier
    * @return Returns the string "Temperature"
    */
    std::string getType() override;
};

/**
 * @class PressureSensor
 * @brief Concrete sensor representing a pressure sensor.
*/
class PressureSensor : public Sensor {
public:
    void setValue(float value) override;
    float readValue() override;

    /**
    * @brief Get the sensor type identifier
    * @return Returns the string "Pressure"
    */
    std::string getType() override;
};

/**
 * @class VibrationSensor
 * @brief Concrete sensor representing a pressure sensor.
*/
class VibrationSensor : public Sensor {
public:
    void setValue(float value) override;
    float readValue() override;

    /**
    * @brief Get the sensor type identifier
    * @return Returns the string "Vibration"
    */
    std::string getType() override;
};

/**
 * @class SensorFactory
 * @brief Factory class to create sensor objects dynamically.
 * 
 * Provides a centralized way to instantiate different types of sensors
 * based on a string identifier. This follows the Factory Method design pattern,
 * allowing the rest of the system to remain decoupled from concrete sensor types.
 */
class SensorFactory {
public:
    /**
     * @brief Create a sensor instance based on a type string.
     * 
     * @param type Type of sensor to create ("Temperature", "Pressure", "Vibration")
     * 
     * @return std::unique_ptr<Sensor> Pointer to the newly created created sensor object.
     *         Returns nullptr if the type is unknown. 
     */
    static std::unique_ptr<Sensor> createSensor(const std::string& type);
};

/**
 * @class Machine
 * @brief Represents a physical machine containing multiple sensors.
 * 
 * The Machine class stores:
 *  - The type of machine (`type`)
 *  - A unique name identifier (`name`)
 *  - A collection of sensors (`sensors`) managed via `std::unique_otr<Sensor>`
 * 
 * Sensors are stored polymorphically, so a machine can hold any type
 * derived from the abstract `Sensor` base class. The class provides
 * methods to interact with sensors without needing to know their concrete types.
*/
class Machine {
private:
    MachineType type;                               /**< Type of the machine */

public:
    std::string name;                               /**< Name of the machine */
    std::vector<std::unique_ptr<Sensor>> sensors;   /**< Polymorphic sensor list */

    /**
     * @brief Construct a Machine object with name, sensor types, and machine type.
     * 
     * Dynamically creates sensors using SensorFactory for each type provided.
     * 
     * @param machineName Name of the machine
     * @param sensorTypes List of sensor type strings ("Temperature", "Pressure", etc.)
     * @param machineType Enum representing the machine type
    */
    Machine(const std::string& machineName, const std::vector<std::string>& sensorTypes, MachineType machineType);
    
    /** @brief Display the machine name and all associated sensors */
    void display() const;

    /** @brief Set the value of a specific sensor by type */
    void setSensorValue(const std::string& type, float value);

    /** @brief Get the value of a specific sensor by type */
    float getSensorValue(const std::string& type);

    /** @brief Get the type of the machine */
    MachineType getType() const { return type; }
};

#endif // SENSOR_H