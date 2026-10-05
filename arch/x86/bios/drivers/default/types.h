#ifndef TYPES_H
#define TYPES_H

typedef __INT8_TYPE__      int8_t;
typedef __INT16_TYPE__     int16_t;
typedef __INT32_TYPE__     int32_t;
typedef __INT64_TYPE__     int64_t;

typedef __UINT8_TYPE__     uint8_t;
typedef __UINT16_TYPE__    uint16_t;
typedef __UINT32_TYPE__    uint32_t;
typedef __UINT64_TYPE__    uint64_t;

typedef __SIZE_TYPE__      size_t;
typedef __UINTPTR_TYPE__   uintptr_t;
typedef __INTPTR_TYPE__    intptr_t;

typedef _Bool bool;
#define true  1
#define false 0
#define __bool_true_false_are_defined 1

#ifndef NULL
#define NULL ((void*)0)
#endif

#endif