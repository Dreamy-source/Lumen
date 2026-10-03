#ifndef HDA_REQUESTS_H
#define HDA_REQUESTS_H

#include "common/baselib.h"

static inline void hda_ignore_req(void)
{
    volatile uint32_t* hda = (volatile uint32_t*)hda_t.bar0;
    uint32_t gtcl = hda[2];

    // UNSOL = 0 (ignore requests to ring buffer)
    gtcl &= (1 << 8);
    hda[2] = gtcl;
}

// UNSOL control
static inline void hda_listen_req(void)
{
    volatile uint32_t* hda = (volatile uint32_t*)hda_t.bar0;
    uint32_t gtcl = hda[2];

    // UNSOL = 1 (listen requests to ring buffer)
    gtcl |= (1 << 8);
    hda[2] = gtcl;
}

#endif