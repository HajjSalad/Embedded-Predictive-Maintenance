#ifndef THREADS_H
#define THREADS_H

/**
 * @file threads.h
 * @brief 
*/

#define THREAD_SENSOR_WRITE_PERIOD_MS       (5000U)
#define THREAD_SENSOR_READ_PERIOD_MS        (5000U)
#define THREAD_ANOMALY_DETECT_PERIOD_MS     (5000U)

// Function Prototypes
void sensor_write(void);
void sensor_read(void);
void anomaly_detect(void);
void anomaly_handle(void);
void system_log(void);

#endif // THREADS_H