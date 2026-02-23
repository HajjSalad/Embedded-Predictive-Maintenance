# SPDX-License-Identifier: Apache-2.0

# Specify the minimum required CMake version
cmake_minimum_required(VERSION 3.20.0)

# Locate the Zephyr build system using the ZEPHYR_BASE environment variable
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

# Define the project name
project(edge_pm)

# Specify the application source files (PRIVATE: means sources only used by this target)
# Apparently GLOB CMake won't automatically re-run if you add a new file, you have to manually re-run cmake once
target_sources(app PRIVATE 
    src/main.c 
    src/utils/demo.cpp 
    src/machines/sensor.cpp 
    src/machines/wrapper.cpp 
    src/threads/thread_sensor_read.c 
    src/threads/thread_sensor_write.c 
    src/threads/thread_anomaly_detect.c 
    src/threads/thread_anomaly_handle.c 
    src/threads/thread_system_logger.c 
    src/core/detection.c
    src/core/circular_buffer.c)

# Include directories
target_include_directories(app PRIVATE include include/core include/machines include/threads include/utils)

# Enable C++ support
set(Zephyr_EXTRA_MODULES app)