#include "efi/efi.h"
#include "efi/efilib.h"
#include <efi/efiapi.h>
#include <efi/efidef.h>
#include <efi/efierr.h>
#include <efi/efiprot.h>
#include <efi/x86_64/efibind.h>

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    InitializeLib(ImageHandle, SystemTable);
    
    
    EFI_GUID gopGuid                     = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL         *gop;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *gopInfo;
    UINTN                                 gopSizeOfInfo, gopModes, gopNativeMode;


    uint64_t fb;
    uint64_t fb_size;
    uint32_t fb_pitch;
    uint32_t fb_x;
    uint32_t fb_y;


    EFI_MEMORY_DESCRIPTOR *mmap       = NULL;
    UINTN                 mmap_size   = 0;
    UINTN                 mmap_key    = 0;
    UINTN                 mmdesc_size = 0;
    UINTN                 mmdesc_ver  = 0;


    EFI_STATUS status = uefi_call_wrapper(
        BS->LocateProtocol,
        3,
        &gopGuid,
        NULL,
        (void**)&gop
    );
    if (EFI_ERROR(status))
    {
        Print(L"[ fail ] Graphics Output Protocol (message from uefi): %r\n", status);
        return status;
    }
    status = uefi_call_wrapper(
        gop->QueryMode,
        4,
        gop,
        gop->Mode==NULL? 0:gop->Mode->Mode,
        &gopSizeOfInfo,
        &gopInfo
    );
    if (status == EFI_NOT_STARTED)
    {
        status = uefi_call_wrapper(
            gop->SetMode,
            2,
            gop,
            0
        );
    }
    if (EFI_ERROR(status))
    {
        Print(L"[ fail ] failed to set native mode (message from uefi): %r\n", status);
    }
    else
    {
        gopNativeMode = gop->Mode->Mode;
        gopModes      = gop->Mode->MaxMode;
    }

    for (int i = 0; i < gopModes; i++)
    {
        status = uefi_call_wrapper(
            gop->QueryMode,
            4,
            gop,
            i,
            &gopSizeOfInfo,
            &gopInfo
        );
        if (gopInfo->HorizontalResolution == 1920 &&
            gopInfo->VerticalResolution == 1080
        )
        {
            Print(L"modinfo: found preferred mode (mode=%03d, %dx%d)\n",
                i,
                gopInfo->HorizontalResolution,
                gopInfo->VerticalResolution
            );
            status = uefi_call_wrapper(
                gop->SetMode, 
                2,
                gop,
                i
            );
            if (EFI_ERROR(status))
            {
                Print(L"[ fail ] SetMode failed (message from uefi): %r\n", status);
            }
            Print(L"\nmodinfo (preferred):\n| mode: %03d\n| width: %d\n| height: %d\n",
                i,
                gopInfo->HorizontalResolution,
                gopInfo->VerticalResolution,
                gopInfo->PixelFormat
            );
            Print(L"\nmodinfo: finish\n");
            break;
        }
        Print(L"\nmodinfo:\n| mode: %03d\n| width: %d\n| height: %d\n",
            i,
            gopInfo->HorizontalResolution,
            gopInfo->VerticalResolution,
            gopInfo->PixelFormat
        );
    }
    fb       = gop->Mode->FrameBufferBase;
    fb_size  = gop->Mode->FrameBufferSize;
    fb_pitch = gop->Mode->Info->PixelsPerScanLine;
    fb_x     = gop->Mode->Info->HorizontalResolution;
    fb_y     = gop->Mode->Info->VerticalResolution;
    Print(L"\nframebuffer:\n| address: %p\n| size: %d\n| width: %d\n| height: %d\n| pxperline: %d\n",
        gop->Mode->FrameBufferBase,
        gop->Mode->FrameBufferSize,
        gop->Mode->Info->HorizontalResolution,
        gop->Mode->Info->VerticalResolution,
        gop->Mode->Info->PixelsPerScanLine
    );
    Print(L"\nframebuffer: finish\n");

    /// Memory Map
    status = uefi_call_wrapper(
        ST->BootServices->GetMemoryMap,
        5,
        &mmap_size,
        NULL,
        &mmap_key,
        &mmdesc_size,
        &mmdesc_ver
    );
    mmap_size += 2 * (mmdesc_size);
    status = uefi_call_wrapper(
        ST->BootServices->AllocatePool,
        3,
        EfiLoaderData,
        mmap_size,
        (void**)&mmap
    );
    if (EFI_ERROR(status))
    {
        Print(L"[ fail ] AllocatePool (message from uefi): %r\n", status);
        return status;
    }
    status = uefi_call_wrapper(
        ST->BootServices->GetMemoryMap, 
        5,
        &mmap_size,
        mmap,
        &mmap_key,
        &mmdesc_size,
        &mmdesc_ver
    );
    if (EFI_ERROR(status))
    {
        Print(L"[ fail ] GetMemoryMap (message from uefi): %r\n", status);
        return status;
    }

    UINTN mmentries = mmap_size / mmdesc_size;
    Print(L"\nmmap:\n| entries: %d\n", mmentries);
    for (UINTN i = 0; i < mmentries; i++)
    {
        EFI_MEMORY_DESCRIPTOR *d = (void*)((UINT8*)mmap + i * (mmdesc_size));

        Print(L"| [%d] type=%d, pad=%d, phys=0x%lx, virt=0x%lx, pages=%ld, attr=0x%lx\n",
            i,
            d->Type,
            d->Pad,
            d->PhysicalStart,
            d->VirtualStart,
            d->NumberOfPages,
            d->Attribute
        );
    }
    Print(L"\nmmap: finish\n");

    /// Exit Boot Services
    /*
    Print(L"\nfinal: ExitBootServices\n");
    status = uefi_call_wrapper(BS->ExitBootServices, 2, ImageHandle, mmap_key);
    if (EFI_ERROR(status))
    {
        Print(L"[ fail ] ExitBootServices (message from uefi): %r\n", status);
        return status;
    }
    */

    while (1)
    {
        status = uefi_call_wrapper(BS->ExitBootServices, 2, ImageHandle, mmap_key);
        if (!EFI_ERROR(status)) break;

        mmap_size = 0;
        uefi_call_wrapper(BS->GetMemoryMap, 5, &mmap_size, NULL, &mmap_key, &mmdesc_size, &mmdesc_ver);
        mmap_size += 2 * (mmdesc_size);
        uefi_call_wrapper(BS->FreePool, 1, mmap);
        uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, mmap_size, (void**)&mmap);
        uefi_call_wrapper(BS->GetMemoryMap, 5, &mmap_size, mmap, &mmap_key, &mmdesc_size, &mmdesc_ver);
    }

    while (1) { __asm__ volatile ("hlt"); }
}