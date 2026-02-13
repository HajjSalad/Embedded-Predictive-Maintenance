## 📈 Data Logger with Anomaly Detection
This project implements a **real-time embedded predictive maintenance system** on **STM32 microcontrollers** using **Zephyr RTOS**.  

It simulates industrial machines equipped with multiple sensors, logs sensor data in real time, and performs **on-device anomaly detection** to identify abnormal operating conditions. The system is designed to be **modular, scalable, and suitable for resource-constrained embedded environments**.

#### 💡 Documentation Guide
- This README provides a high-level overview of the project's goals, architecture, and core features.  
- Detailed development notes, design decisions, and implementation reasoning are documented in [**Notion Workspace**](https://hajjsalad.notion.site/Data-Logger-and-Detection-1c3a741b5aab8008b104dcbf9cf744c8)
- Feature-specific explanations and deep technical breakdowns are linked directly within the relevant sections below.
- Complete API and module documentation is automatically generated using **Doxygen Documentaion**.
  - The generated documentation is hosted on [GitHub Pages]()
---
### 🗝️ Key Features
1. **Zephyr RTOS-Based Embedded Architecture**    
- Built on Zephyr RTOS running on STM32 microcontrollers
- Leverages Zephyr's kernel primitives for threading, synchronization, and timing
- Designed for deterministic execution and portability across supported boards
- Zephyr RTOS in STM32 Setup Guide: [STM32 + Zephyr + VSCode](link)   
`Zephyr RTOS` · `STM32` · `Embedded Systems` · `Real-Time Operating Systems`  
2. **Object-Oriented Machine & Sensor Modeling**   
- Uses Object-Oriented Design to model Machines and Sensors, enabling polymorphic access to different sensor types
- Implements the Factory Method Pattern to dynamically create sensors at runtime based on type identifiers
- Combines C++ core logic with C-compatible wrapper APIs, allowing seamless integration with Zephyr's C-based ecosystem  
`C / C++` · `OOP` · `Design Patterns` · `Factory Method` · `C/C++ Interoperability` 
3. **Multithreaded Data Pipeline**
- Multiple worker threads perform independent tasks including sensor updates, data collection, and anomaly detection
- Threads emit structured log events to a shared message queue
- A dedicated logger thread serializes and prints all output, preventing race conditions on the terminal  
`Multithreading` · `Producer–Consumer Pattern` · `Message Queues` · `Thread Synchronization`
4. **On-Device Anomaly Detection**
- Sensor readings are continuously compared against defined normal operating ranges
- Statistical detection logic identifies deviations indicating abnormal behavior
- Anomaly handling is event-driven, minimizing unnecessary CPU usage  
`Anomaly Detection` · `Edge Computing` · `Predictive Maintenance` · `Statistical Analysis`
5. **Doxygen Documentation**   
- Fully documented using Doxygen with clear function, module, and data structure description.
- Generate browsable HTML documentation published via GitHub Pages from the `docs/` directory.  
`Documentation` · `Maintainability` · `Code Quality`
---
### 🏗 System Architecture
```
[📡 Sensors] → [🧵 Threads] → [💾 Buffer] → [🤖 Detection] → [📊 Logging]
     ↓             ↓              ↓              ↓              ↓
  Simulated    Data Pipeline   Circular    Statistical    Centralized
   Values      (5 Threads)      Buffer      Analysis        Output
```
### 🧶 Threading Model
The system uses 5 concurrent threads with different priorities to implement a staged data processing pipeline:   
1. Thread 1: `sensor_data_writer`  `Priority = 7`  
- Generates simulated sensor values within realistic operating ranges
- Writes values into sensor objects via the Factory Pattern interface
- Emits log events describing written values
2. Thread 2: `data_collector`  `Priority = 6`  
- Reads sensor data from sensor objects
- Writes collected data into a circular buffer for time-series analysis
- Emits log events to verify data flow correctness
3. Thread 3: `anomaly_detector`  `Priority = 5`  
- Consumes data from the circular buffer
- Performs statistical anomaly detection (range checking, threshold comparison)
- Emits log events describing detection results (normal/anomaly)
- Signal `anomaly_handler` via semaphore when an anomaly is detected
4. Thread 4: `anomaly_handler`  `Priority = 4`  
- Sleeps until signaled by `anomaly_detector`
- When activated:
  - Dumps circular buffer contents to terminal
  - Logs anomaly details with timestamp
  - Emits alert messages
5. Thread 5: `system_logger`  `Priority = 3`. 
- Sole owner of terminal output — prevents race conditions
- Consumes structured log messages from a thread-safe queue
- Prints logs in a serialized manner with thread identification tags
- Example Output:
  ```
  [Thread 1] Machine 1 - Temp: 75.3°C, Pressure: 120psi, Vibration: 1.2mm/s
  [Thread 2] Added to buffer: 75.3
  [Thread 3] Running inference... MSE: 0.023 ✅ Normal
  [Thread 3] ANOMALY DETECTED! MSE: 0.087 ⚠️
  [Thread 4] ⚠️⚠️⚠️ ANOMALY ALERT ⚠️⚠️⚠️
  [Thread 4] Buffer dump: [72.1, 73.5, 74.8, 78.2, 85.4, 90.1, 95.7]
  ```
### ⛓ Synchronization Mechanisms
| Mechanism | Purpose | Protected Resource |
|-----------|---------|-------------------|
| **Mutex** | Protect sensor object access | Thread 1 (write) vs Thread 2 (read) |
| **Mutex** | Protect circular buffer | Thread 2 (write) vs Threads 3/4 (read) |
| **Semaphore** | Signal anomaly detection | Thread 3 → Thread 4 |
| **Message Queue** | Centralized logging | All threads → Thread 5 |
### 🛠️ Machine-Sensor Configuration
Each industrial machine is equipped with specific sensors for predictive maintenance: 

1. Air Compressor (`AIR_COMPRESSOR`)
- Temperature: `60-100°C` • Pressure: `72-145 psi` • Vibration: `0.5-2.0 mm/s`   
- _Monitors overheating, pressure fluctuations, and mechanical wear_ 
 
2. Steam Boiler (`STEAM_BOILER`)
- Temperature: `150-250°C` • Pressure: `87-360 psi`     
- _Tracks thermal efficiency and safety thresholds_

3. Electric Motor (`ELECTRIC_MOTOR`) 
- Temperature: `60-105°C`      
- _Winding insulation safety range_  
---
### 📂 Project Code Structure
```
📁 edge_pm/
│
├── 📁 src/                           Core application source files
│   ├── 📄 main.c                     Zephyr application entry point & initialization
│   ├── 📄 demo.cpp                   C++/C interop demonstration module
│   ├── 📄 sensor.cpp                 Sensor class implementations (C++)
│   ├── 📄 sensor_wrapper.c           C wrapper API for sensor objects
│   ├── 📄 machines.c                 Machine generation and configuration
│   ├── 📄 threads.c                  Thread definitions and lifecycle management
│   ├── 📄 queue.c                    Message queue for centralized logging
│   ├── 📄 circular_buffer.c          Ring buffer for time-series sensor data
│   └── 📄 detection.c                Anomaly detection logic
│
├── 📁 include/                       Public header files
│   ├── 📄 demo.h                     Demo module interface
│   ├── 📄 sensor.h                   Sensor class declarations and public interface
│   ├── 📄 sensor_wrapper.h           C-compatible sensor API
│   ├── 📄 machines.h                 Machine configuration and management
│   ├── 📄 threads.h                  Thread function prototypes
│   ├── 📄 queue.h                    Logging queue interface
│   ├── 📄 circular_buffer.h          Circular buffer interface
│   └── 📄 detection.h                Anomaly detection interface
│
├── 📄 CMakeLists.txt                 Build configuration
├── 📄 prj.conf                       Zephyr kernel and module configuration
├── 📄 Doxyfile                       Doxygen documentation generation config
└── 📄 README.md                      Project overview and documentation
```
