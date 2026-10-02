#ifndef HDA_H
#define HDA_H

#define HDA_ID 0x26688086

#include "common/types.h"
#include "common/pci/pci.h"
#include "video/vga.h"

static uint32_t hda_devaddr;
static inline void hda_locate()
{
    hda_devaddr = pci_locate_dev32(HDA_ID, 0);

    if (hda_devaddr == PCI_NOTFOUND)
    {
        kprintf("[HDA] %s\n", 0x07, 0x0C, "not found");
        hda_t.dev = 0xFF;
        hda_t.func = 0xFF;
        return;
    }

    hda_t.dev = (hda_devaddr >> 8) & 0xFF;
    hda_t.func = hda_devaddr & 0xFF;
    hda_t.bar0 = pci_read32(0, hda_t.dev, hda_t.func, 0x10);
}

static inline void hda_enable_mmio(uint8_t dev, uint8_t func)
{
    uint32_t cmd = pci_read32(0, dev, func, 0x04);

    // bit 1 - Memory Space Enable
    if (!(cmd & 0x02))
    {
        pci_write32(0, dev, func, 0x04, cmd | 0x02);
    }
}

static inline void hda_ini()
{
    volatile uint32_t* hda = (volatile uint32_t*)hda_t.bar0;

    uint32_t gcap = hda[0];

    uint32_t gtcl = hda[2];
    uint32_t statests = (hda[3] >> 16) & 0x7FFF;

    uint8_t output_streams = (gcap >> 12) & 0x0F;
    uint8_t input_streams = (gcap >> 8) & 0x0F;
    uint8_t bidirectional_streams = (gcap >> 3) & 0x1F;
    uint8_t serial_data_out_signals = (gcap >> 1) & 0x03;
    uint8_t bit64_supported = (gcap >> 0);
    
    uint8_t vmin = *(volatile uint8_t*)((uint8_t*)hda + 0x02);
    uint8_t vmaj = *(volatile uint8_t*)((uint8_t*)hda + 0x03);
    uint8_t outpay = *(volatile uint8_t*)((uint8_t*)hda + 0x04);
    uint8_t inpay = *(volatile uint8_t*)((uint8_t*)hda + 0x06);

    kprintf("[HDA] bar0:                     %h\n", 0x07, 0x0F, hda_t.bar0);
    kprintf("[HDA] GCAP:                     %h\n", 0x07, 0x0F, gcap);
    kprintf("[HDA] output streams:           %d\n", 0x07, 0x0F, output_streams);
    kprintf("[HDA] input streams:            %d\n", 0x07, 0x0F, input_streams);
    kprintf("[HDA] bidirectional streams:    %d\n", 0x07, 0x0F, bidirectional_streams);
    kprintf("[HDA] serial data out signals:  %d\n", 0x07, 0x0F, serial_data_out_signals);
    kprintf("[HDA] 64 bit address supported: %s\n", 0x07, 0x0F, bit64_supported ? "yes" : "no");
    kprintf("[HDA] version:                  %d.%d\n", 0x07, 0x0F, vmaj, vmin);
    kprintf("[HDA] outpay:                   %d words\n", 0x07, 0x0F, outpay);
    kprintf("[HDA] inpay:                    %d words\n", 0x07, 0x0F, inpay);
}

static inline void hda_disable(void)
{
    volatile uint32_t* hda = (volatile uint32_t*)hda_t.bar0;
    uint32_t gtcl = hda[2];

    // controller reset (crst = 0)
    gtcl &= ~0x1;
    hda[2] = gtcl;  // write
}

static inline void hda_enable(void)
{
    volatile uint32_t* hda = (volatile uint32_t*)hda_t.bar0;
    uint32_t gtcl = hda[2];

    // controller exit reset state (crst = 1)
    gtcl &= ~0x0;
    hda[2] = gtcl;
}

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

static inline void hda_flush(void)
{
    volatile uint32_t* hda = (volatile uint32_t*)hda_t.bar0;

    // flush control = 1 (FCTRL, flush)
    uint32_t gtcl = hda[2];
    hda[2] = gtcl;

    while (!(*(volatile uint16_t*)((uint8_t*)hda + 0x10) & (1 << 10))){}
}

#endif