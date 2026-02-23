/**
 * @file demo.cpp
 * @brief Simple C++ demo module exposed to C code.
 *
 * This file demonstartes how C++ functionality can be safely 
 * used in a Zephyr project and accessed from C code.
*/

#include "demo.h"
#include <string>

/**
 * @class Demo
 * @brief Internal C++ helper class.
 * 
 * This class is not exposed outside this file.
 * It encapsulates C++-only logic and returns a std::string
*/
class Demo {
public:
    /**
     * @brief Get a demo message.
     * @return std::string containing a demo message
     */
    std::string getMessage() {
        return "Hello from C++!";
    }
};

/** @brief Global instance of the Demo class */
static Demo demoInstance;

/**
 * @brief Get a demo message as a C-style string.
 * 
 * This function bridges C++ std::string with C code by 
 * returning a null-terminated string (const char*)
 * 
 * @note The returned pointer remains valid because the 
 * underlying std::string is declared static.
 * 
 * @return Pointer to a null-terminated string
 */
const char* demo_get_message(void) {
    static std::string message = demoInstance.getMessage();
    return message.c_str();
}
