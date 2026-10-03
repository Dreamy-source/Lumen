#ifndef HDA_STRUCT_INFO_H
#define HDA_STRUCT_INFO_H

#include "common/baselib.h"

typedef struct {
    uint8_t   dev;
    uint8_t   func;
    uintptr_t bar0;
} hda_device_t;

static hda_device_t hda_t;

#endif