/**
 * @file cep.h
 * @brief Custom utility functions for CEP project
 * @details Provides float-to-string conversion without printf float support
 */

#ifndef CEP_H
#define CEP_H

#include <stdint.h>

/**
 * @brief Convert float to string with 1 decimal place
 * @param value: Float value to convert
 * @param buffer: Output buffer (must be at least 10 bytes)
 * @param decimals: Number of decimal places (0-2 supported)
 * 
 * EXAMPLE:
 *   char str[16];
 *   float_to_string(25.7, str, 1);  // Result: "25.7"
 *   float_to_string(99.99, str, 2); // Result: "99.99"
 */
void float_to_string(float value, char *buffer, uint8_t decimals);

/**
 * @brief Convert float temperature to string with °C suffix
 * @param temp: Temperature value
 * @param buffer: Output buffer (must be at least 16 bytes)
 * 
 * EXAMPLE:
 *   char temp_str[16];
 *   temp_to_string(28.5, temp_str);  // Result: "28.5C"
 */
void temp_to_string(float temp, char *buffer);

#endif /* CEP_H */
