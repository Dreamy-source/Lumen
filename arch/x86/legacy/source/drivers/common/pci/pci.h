#ifndef PCI_H
#define PCI_H

#define ENABLE_BIT     0x80000000u
#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA    0xCFC

#include "common/types.h"
#include "common/port.h"

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

static inline uint32_t pci_read32(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset)
{
    uint32_t address =
    ((uint32_t)bus      << 16) |
    ((uint32_t)device   << 11) |
    ((uint32_t)function << 8)  |
    ((uint32_t)offset & 0xFC)  |
    ENABLE_BIT;

    outl(CONFIG_ADDRESS, address);
    return inl(CONFIG_DATA);
}

static inline uint16_t pci_read16(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset)
{   
    uint32_t value = pci_read32(bus, device, function, offset);
    return (uint16_t)(value >> (offset & 2) * 8);
}

static inline uint8_t pci_read8(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset)
{
    uint32_t value = pci_read32(bus, device, function, offset);
    return (uint8_t)(value >> (offset & 3) * 8);
}

static inline void pci_write32(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset, uint32_t value)
{
    uint32_t address =
    ((uint32_t)bus      << 16) |
    ((uint32_t)device   << 11) |
    ((uint32_t)function << 8)  |
    ((uint32_t)offset & 0xFC)  |
    ENABLE_BIT;

    outl(CONFIG_ADDRESS, address);
    outl(CONFIG_DATA, value);
}

static inline void pci_write16(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset, uint16_t value)
{
    uint32_t old = pci_read32(bus, device, function, offset);
    uint32_t shift = (offset & 2) * 8;
    uint32_t mask = 0xFFFFu << shift;
    uint32_t value32 = (old & ~mask) | (uint32_t)value << shift;
    pci_write32(bus, device, function, offset, value32);
}

static inline void pci_write8(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset, uint8_t value)
{
    uint32_t old = pci_read32(bus, device, function, offset);
    uint32_t shift = (offset & 3) * 8;
    uint32_t mask = 0xFFu << shift;
    uint32_t value32 = (old & ~mask) | (uint32_t)value << shift;
    pci_write32(bus, device, function, offset, value32);
}
 
static inline uint16_t pci_device_exists(uint8_t bus, uint8_t device, uint8_t function)
{
    return pci_read16(bus, device, function, 0x00) != 0xFFFF;
}

#endif