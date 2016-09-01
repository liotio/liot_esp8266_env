#ifndef USER_TYPES_H
#define USER_TYPES_H

/**
 * Some typedefs are missing in c_types.h
 */
typedef unsigned short      int16;
typedef signed long long    int64;

/**
 * hw timer source types are not accessible
 */
typedef enum {
    FRC1_SOURCE = 0,
    NMI_SOURCE = 1,
} FRC1_TIMER_SOURCE_TYPE;

#endif
