#ifndef DEMO_H
#define DEMO_H

/**
 * @file demo.h
 * @brief Header for the Demo C++ module exposed to C code.
*/

#ifdef __cplusplus
extern "C" {
#endif

// Function prototypes
const char* demo_get_message(void);

#ifdef __cplusplus
}
#endif

#endif // DEMO_H