#ifndef TYPES_H
#define TYPES_H

typedef signed char        int8_t;
typedef signed short       int16_t;
typedef signed int         int32_t;
typedef signed long long   int64_t;

typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

typedef __SIZE_TYPE__       size_t;
typedef __UINTPTR_TYPE__    uintptr_t;
typedef __INTPTR_TYPE__     intptr_t;

#define NULL ((void*)0)

#endif