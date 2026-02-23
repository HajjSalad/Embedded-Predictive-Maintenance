#ifndef SENSOR_H
#define SENSOR_H

/**
 * @file sensor.h
 * @brief 
 * 

*/

#include <stdint.h>

#include "wrapper.h"

#define MAX_SENSORS         3U      // Max sensors per machine
#define MAX_TEMP_SENSORS    3U      // Air Compressor + Steam Boiler + Electric Motor
#define MAX_PRESS_SENSORS   2U      // Air Compressor + Steam Boiler
#define MAX_VIB_SENSORS     1U      // Air Compressor only

// Abstract base class for all sensor types
class Sensor {
protected:
    uint16_t sensorNumber;                                  // Unique ID for the sensor
    float    sensorValue;                                   // Current sensor reading
    float    minValue;
    float    maxValue;

public:
    explicit Sensor(uint16_t number, float min, float max) 
        : sensorNumber(number), sensorValue(0.0f), minValue(min), maxValue(max) {}              // Constructor

    void setValue(float value) { sensorValue = value; }; // Concrete implementation -> all sensors set the same
    virtual float readValue() = 0;                       // pure virtual -> read a value is sensor-specific
    virtual const char* getType() const = 0;

    float getMinValue() const { return minValue; }
    float getMaxValue() const { return maxValue; }

    virtual ~Sensor() = default;                            // Virtual destructor for proper cleanup
};

// Temperature sensor
class TempSensor : public Sensor {
public:
    explicit TempSensor(uint16_t number, float min, float max) 
        : Sensor(number, min, max) {}
    float readValue() override { return sensorValue; }                  // Provide own implementation
    const char* getType() const override { return "Temperature"; }
};

// Pressure sensor
class PressureSensor : public Sensor {
public:
    explicit PressureSensor(uint16_t number, float min, float max) 
        : Sensor(number, min, max) {}
    float readValue() override { return sensorValue; }
    const char* getType() const override { return "Pressure"; }
};

// Vibration sensor
class VibrationSensor : public Sensor {
public:
    explicit VibrationSensor(uint16_t number, float min, float max)
        : Sensor(number, min, max) {}
    float readValue() override { return sensorValue; }
    const char* getType() const override { return "Vibration"; }
};

// Factory class
class SensorFactory {
public:
    static Sensor* createSensor(const char* type, uint16_t sensorNumber, 
                                float minValue, float maxValue);
};

class Machine {
private:
    Sensor* sensors[MAX_SENSORS];
    uint8_t sensorCount;

public: 
    const char* name;
    MachineType type;

    explicit Machine(const char* machineName, MachineType machineType);

    void addSensor(Sensor* sensor);
    void setSensorValue(const char* sensorType, float value);
    float getSensorValue(const char* sensorType) const;
    float getSensorMinValue(const char* sensorType) const;
    float getSensorMaxValue(const char* sensorType) const;
    const char* getSensorType(uint8_t sensorIndex) const;

    const char* getName() const { return name; }
    MachineType getType() const { return type; }
    uint8_t getSensorCount() const { return sensorCount; }
};


#endif // SENSOR_H