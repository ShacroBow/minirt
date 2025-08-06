#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

// Simple atof implementation
// Handles optional sign, integer part, decimal part, no exponent

double ft_atof(const char *str)
{
    double result = 0.0;
    double sign = 1.0;
    double fraction = 0.0;
    double divisor = 1.0;
    bool has_fraction = false;

    // Skip whitespace
    while (isspace(*str)) str++;
    // Handle sign
    if (*str == '-') { sign = -1.0; str++; }
    else if (*str == '+') { str++; }
    // Integer part
    while (isdigit(*str)) {
        result = result * 10.0 + (*str - '0');
        str++;
    }
    // Fractional part
    if (*str == '.') {
        str++;
        has_fraction = true;
        while (isdigit(*str)) {
            fraction = fraction * 10.0 + (*str - '0');
            divisor *= 10.0;
            str++;
        }
    }
    result += has_fraction ? fraction / divisor : 0.0;
    return sign * result;
}
