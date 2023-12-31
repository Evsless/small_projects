#ifndef __STDTYPES_H__
#define __STDTYPES_H__
#include <stdint.h>

#define size_t unsigned long

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef enum boolean {
    FALSE,
    TRUE
} boolean_t;

typedef enum stdstatus {
    STATUS_OK,
    STATUS_NOT_OK
} stdstatus_t;

#endif /* __STDTYPES_H__ */