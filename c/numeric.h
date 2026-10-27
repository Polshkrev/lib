#ifndef NUMERIC_H_
#define NUMERIC_H_

#include "doc.hpp" // ! NEEDED FOR NAMESPACE DOCUMENTATION

/**
 * @brief Determine the minimum value between two given numbers.
 * @param a First value to check.
 * @param b Second value to check.
 * @returns `a` if `a` is less than `b`, else `b`.
 */
#define NUMERIC_MIN(a, b) ((a) < (b) ? (a) : (b))

/**
 * @brief Determine the maximum value between two given numbers.
 * @param a First value to check.
 * @param b Second value to check.
 * @returns `a` if `a` is more than `b`, else `b`.
 */
#define NUMERIC_MAX(a, b) ((a) > (b) ? (a) : (b))

/**
 * @brief Clamp a given value to a given minimum or maximum value.
 * @param value Value to clamp.
 * @param minimum Minimum value to check.
 * @param maximum Maximum value to check.
 * @returns The given value clamped to either the minimum or maximum.
 */
#define NUMERIC_CLAMP(value, minimum, maximum) (NUMERIC_MIN(NUMERIC_MAX((value), (minimum)), (maximum)))

#endif // NUMERIC_H_