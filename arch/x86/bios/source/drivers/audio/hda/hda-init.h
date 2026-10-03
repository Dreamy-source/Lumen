#ifndef HDA_INIT_H
#define HDA_INIT_H

#include "common/baselib.h"
#include "audio/hda/hda-locate.h"
#include "audio/hda/hda-mmio.h"
#include "audio/hda/hda-control.h"
#include "audio/hda/hda-requests.h"
#include "audio/hda/hda-gi.h"
#include "audio/hda/hda-ci.h"
#include "audio/hda/hda-si.h"


static inline void hda_ini(void)
{
    volatile uint32_t* hda = (volatile uint32_t*)hda_t.bar0;

    uint32_t gcap = hda[0];

    uint32_t gtcl = hda[2];
    uint32_t statests = *(volatile uint16_t*)((uint8_t*)hda + 0x0E);

    uint8_t output_streams = (gcap >> 12) & 0x0F;
    uint8_t input_streams = (gcap >> 8) & 0x0F;
    uint8_t bidirectional_streams = (gcap >> 3) & 0x1F;
    uint8_t serial_data_out_signals = (gcap >> 1) & 0x03;
    uint8_t bit64_supported = (gcap >> 0);
    
    uint8_t vmin = *(volatile uint8_t*)((uint8_t*)hda + 0x02);
    uint8_t vmaj = *(volatile uint8_t*)((uint8_t*)hda + 0x03);
    uint8_t outpay = *(volatile uint8_t*)((uint8_t*)hda + 0x04);
    uint8_t inpay = *(volatile uint8_t*)((uint8_t*)hda + 0x06);
    uint16_t outstrmpay = *(volatile uint16_t*)((uint8_t*)hda + 0x18);
    uint16_t instrmpay = *(volatile uint16_t*)((uint8_t*)hda + 0x1A);

    for (int i = 0; i < 15; i++)
    {
        if (statests & (1 << i))
        {
            kprintf("[HDA] codec on %s%d (addr %h)\n", 0x07, 0x0F, "SDI", i, i);
        }
    }
    kprintf("[HDA] BAR0:                     %h\n", 0x07, 0x0F, hda_t.bar0);
    kprintf("[HDA] GCAP:                     %h\n", 0x07, 0x0F, gcap);
    kprintf("[HDA] Output streams:           %d\n", 0x07, 0x0F, output_streams);
    kprintf("[HDA] Input streams:            %d\n", 0x07, 0x0F, input_streams);
    kprintf("[HDA] Bidirectional streams:    %d\n", 0x07, 0x0F, bidirectional_streams);
    kprintf("[HDA] Serial data out signals:  %d\n", 0x07, 0x0F, serial_data_out_signals);
    kprintf("[HDA] 64 bit address supported: %s\n", 0x07, 0x0F, bit64_supported ? "yes" : "no");
    kprintf("[HDA] Version:                  %d.%d\n", 0x07, 0x0F, vmaj, vmin);
    kprintf("[HDA] Outpay:                   %d words\n", 0x07, 0x0F, outpay);
    kprintf("[HDA] Inpay:                    %d words\n", 0x07, 0x0F, inpay);
    kprintf("[HDA] STATESTS:                 %h\n", 0x07, 0x0F, statests);
    kprintf("[HDA] OUTSTRMPAY:               %d\n", 0x07, 0x0F, outstrmpay);
    kprintf("[HDA] INSTRMPAY:                %d\n", 0x07, 0x0F, instrmpay);
}

#endif