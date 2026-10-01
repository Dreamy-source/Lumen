#ifndef PCI_H
#define PCI_H

#define ENABLE_BIT     0x80000000u
#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA    0xCFC
#define PCI_NOTFOUND   0xFFFFFFFF;

#include "video/vga.h"
#include "common/types.h"
#include "common/port.h"
#include "common/string.h"

// bus 0 (root)
// ├─ GPU
// └─ PCI-PCI bridge
//    │ 
//    └─ bus 1
//       ├─ USB
//       ├─ SATA
//       └─ Audio
//

// bus (0-255) - шина
// device|slot (0-31) - устройство на шине
// function (0-7) - функция устройства
// offset (0-255) - что читать (ex: 0x00 = vendor ID)

// 31:    enabled/disabled
// 30-24: reserved
// 23-16: bus
// 15-11: device
// 10-8:  function
// 7-0:   offset

// Vendor ID - ex: Intel / NVIDIA
// Device ID - device name
// Class - device type (ex: 0x01: mass storage, 0x04: Multimedia)
// Subclass - subcategory of class (ex: Class.0x01.0x08 (NVMe))
// Header Type - format (ex: 0x00 - classic device, 0x01 - PCI-PCI bridge, 0x02 - CardBus bridge)
// BAR - device address
// Secondary Bus - bridges

// | Host Bridge       (bus 0) (device ?) (function 0)
// | ISA Bridge        (bus 0) (device ?) (function 0)
// | VGA Controller    (bus 0) (device ?) (function 0)
// | PCI-to-PCI bridge (bus 0) (device ?) (function 0)
// | USB Controller    (bus 0) (device ?) (function 0)
// | SATA Controller   (bus 0) (device ?) (function 0)
// | Audio Controller  (bus 0) (device ?) (function 0)
// | Ethernet          (bus 0) (device ?) (function 0)

/*
PCI
│
└── bus 0
    │
    └── device 6
        │
        └── function 0
            │
            ├── offset 0x00               ← Vendor ID + Device ID
            │   ├── Vendor = 0x8086       (Intel)
            │   └── Device = 0x2668       (Intel HDA)
            │
            ├── offset 0x08               ← Class Code + Subclass
            │   ├── Class = 0x04          (Multimedia)
            │   └── Subclass = 0x03       (Audio)
            │
            └── offset 0x10               ← BAR0
                └── BAR0 = 0xFEBF0000     (MMIO HDA)
*/

/*
    BAR1 - 0x14
    BAR2 - 0x18
    BAR3 - 0x1C
    BAR4 - 0x20
    BAR5 - 0x24
*/

// class code examples:
            //   0x01 - Mass Storage
            //   0x02 - Network
            //   0x03 - Display
            //   0x04 - Multimedia
            //   0x05 - Memory Controller
            //   0x06 - Bridge

            // subclass examples:
            //   Mass Storage:
            //     0x00 - SCSI
            //     0x01 - IDE
            //     0x02 - Floppy
            //     0x03 - IPI
            //     0x04 - RAID
            //     0x05 - ATA
            //     0x06 - SATA
            //     0x07 - Serial Attached SCSI
            //     0x08 - NVMe
            //   Multimedia:
            //     0x00 - Video
            //     0x01 - Audio (old)
            //     0x02 - Telephony
            //     0x03 - Audio (HDA)

typedef struct {
    uint8_t dev;
    uint8_t func;
} hda_device_t;

static inline uint32_t pci_read32(uint8_t bus, uint8_t dev, uint8_t func, uint8_t offset)
{
    uint32_t address =
        ((uint32_t)bus    << 16)   |
        ((uint32_t)dev    << 11)   |
        ((uint32_t)func   << 8)    |
        ((uint32_t)offset & 0xFC)  |
        ((uint32_t)ENABLE_BIT);

        outl(CONFIG_ADDRESS, address);
        return inl(CONFIG_DATA);
}

static inline void pci_write32(uint8_t bus, uint8_t dev, uint8_t func, uint8_t offset, uint32_t val)
{
    uint32_t address =
        ((uint32_t)bus    << 16)   |
        ((uint32_t)dev    << 11)   |
        ((uint32_t)func   << 8)    |
        ((uint32_t)offset & 0xFC)  |
        ((uint32_t)ENABLE_BIT);

        outl(CONFIG_ADDRESS, address);
        outl(CONFIG_DATA, val);
}

static inline uint32_t pci_device_exists32(uint8_t bus, uint8_t dev)
{
    return pci_read32(bus, dev, 0, 0) != 0xFFFFFFFF;
}

static inline uint32_t pci_locate_dev32(uint32_t prefdev, uint8_t bus)
{
    for (uint8_t dev = 0; dev < 32; dev++)
    {
        for (uint8_t func = 0; func < 8; func++)
        {
            uint32_t vendor_device = pci_read32(bus, dev, func, 0x00);
            uint16_t vendor = vendor_device & 0xFFFF;

            if (vendor == 0xFFFF) { continue; }

            if (vendor_device == prefdev)
            {
                kprintf("[PCI] found vendor=%h, prefdev=%h\n", 0x07, 0x0F, vendor_device, prefdev);
                hda_device_t hda;
                hda.dev = dev;
                hda.func = func;
                return (dev << 8) | func;
            }
        }
    }
    return PCI_NOTFOUND;
}

#endif