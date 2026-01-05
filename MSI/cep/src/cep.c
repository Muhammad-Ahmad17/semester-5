/**
 * @file cep.c
 * @brief Custom utility functions implementation
 * @details Lightweight float-to-string conversion without printf float support
 */

#include "cep.h"
#include <stdint.h>

/**
 * @brief Convert integer to string (helper function)
 * @param value: Integer value
 * @param buffer: Output buffer
 * @return Length of string
 */
static uint8_t int_to_string(int32_t value, char *buffer)
{
    uint8_t i = 0;
    uint8_t is_negative = 0;
    
    /* Handle negative numbers */
    if (value < 0) {
        is_negative = 1;
        value = -value;
    }
    
    /* Handle zero case */
    if (value == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return i;
    }
    
    /* Convert digits (reverse order) */
    uint8_t start = i;
    while (value > 0) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }
    
    /* Add negative sign */
    if (is_negative) {
        buffer[i++] = '-';
    }
    
    /* Reverse the string */
    uint8_t end = i - 1;
    start = is_negative ? 1 : 0;
    while (start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
    
    buffer[i] = '\0';
    return i;
}

/**
 * @brief Convert float to string with specified decimal places
 * @param value: Float value to convert
 * @param buffer: Output buffer (must be at least 10 bytes)
 * @param decimals: Number of decimal places (0-2 supported)
 */
void float_to_string(float value, char *buffer, uint8_t decimals)
{
    /* Handle negative values */
    uint8_t is_negative = 0;
    if (value < 0) {
        is_negative = 1;
        value = -value;
        *buffer++ = '-';
    }
    
    /* Extract integer part */
    int32_t int_part = (int32_t)value;
    
    /* Extract fractional part */
    float frac_part = value - (float)int_part;
    
    /* Convert integer part */
    uint8_t len = int_to_string(int_part, buffer);
    buffer += len;
    
    /* Add decimal point and fractional part */
    if (decimals > 0) {
        *buffer++ = '.';
        
        /* Scale fractional part based on decimal places */
        if (decimals == 1) {
            frac_part *= 10.0f;
        } else if (decimals == 2) {
            frac_part *= 100.0f;
        }
        
        int32_t frac_int = (int32_t)(frac_part + 0.5f);  /* Round */
        
        /* Handle carry-over from rounding */
        if (decimals == 1 && frac_int >= 10) {
            frac_int = 0;
        } else if (decimals == 2 && frac_int >= 100) {
            frac_int = 0;
        }
        
        /* Convert fractional digits */
        if (decimals == 1) {
            *buffer++ = '0' + (frac_int % 10);
        } else if (decimals == 2) {
            *buffer++ = '0' + ((frac_int / 10) % 10);
            *buffer++ = '0' + (frac_int % 10);
        }
    }
    
    *buffer = '\0';
}

/**
 * @brief Convert temperature float to string with °C suffix
 * @param temp: Temperature value
 * @param buffer: Output buffer (must be at least 16 bytes)
 */
void temp_to_string(float temp, char *buffer)
{
    float_to_string(temp, buffer, 1);
    
    /* Find end of string */
    while (*buffer != '\0') {
        buffer++;
    }
    
    /* Add 'C' suffix */
    *buffer++ = 'C';
    *buffer = '\0';
}
