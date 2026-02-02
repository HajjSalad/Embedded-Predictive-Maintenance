#ifndef SENSOR_WRAPPER_H
#define SENSOR_WRAPPER_H

/**
 * @file sensor_wrapper.h
 * @brief C-compatible wrapper interface for Machine and Sensor system.
*/
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @enum MachineType
 * @brief Enumeration of supported machine types.
 * 
 * Used to classify machines at creation time and determine their behavior
 * or sensor configuration
*/
typedef enum {
    AIR_COMPRESSOR,
    STEAM_BOILER,
    ELECTRIC_MOTOR
} MachineType;

/**
 * @struct SensorConfig 
 * @brief Configuration data for a single sensor.
 * 
 * Describes valid operating ranges for a sensor type.
*/
typedef struct {
    const char* name;     /**< Sensor name or type identifier */
    float min_value;      /**< Minimum valid sensor value */
    float max_value;      /**< Maximum valid sensor value */
} SensorConfig;

/**
 * @struct MachineConfig
 * @brief Configuration decribing a machine and its sensors.
 * 
 * Defines the sensors attached to a machine and how many are active.
*/
typedef struct {
    const char* name;                 /**< Machine name */
    SensorConfig sensors[3];          /**< List of attached sensors */
    int num_sensors;                  /**< Number of valid sensors */
} MachineConfig;

/**
 * @typedef MachineHandle
 * @brief Opaque handle to a mcahine instance.
 * 
 * Represents an internal C++ Machine object without exposing its 
 * implementation details to C code.
*/
typedef void* MachineHandle;

// Function Prototypes
MachineHandle create_machine(const char* machine_name, MachineType type);
void destroy_machine(MachineHandle handle);
void describe_machine(MachineHandle handle);
void set_sensor_value(MachineHandle handle, const char* sensor_type, float value);
float get_sensor_value(MachineHandle handle, const char* sensor_type);
const char* get_machine_type_string(MachineType type);
MachineType get_machine_type(MachineHandle handle);

#ifdef __cplusplus
}
#endif

#endif // SENSOR_WRAPPER_H