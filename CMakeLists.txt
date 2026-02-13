# SPDX-License-Identifier: Apache-2.0

# Specify the minimum required CMake version
cmake_minimum_required(VERSION 3.20.0)

# Locate the Zephyr build system using the ZEPHYR_BASE environment variable
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

# Define the project name
project(edge_pm)

# Specify the application source files (PRIVATE: means sources only used by this target)
target_sources(app PRIVATE src/main.c src/demo.cpp src/machines.c src/sensor.cpp src/sensor_wrapper.cpp src/threads.c)

# Include directories
target_include_directories(app PRIVATE include)

# Enable C++ support
set(Zephyr_EXTRA_MODULES app)