#include "efi/efi.h"
#include "efi/efilib.h"

typedef struct {
    void* rsdp_addr;
} boot_info;

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
    InitializeLib(ImageHandle, SystemTable);
    
    EFI_GUID                              gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* gopInfo;
    EFI_GRAPHICS_OUTPUT_PROTOCOL*         gop;
    UINTN                                 gopInfoSize, gopModes, gopNativeMode;

    EFI_MEMORY_DESCRIPTOR*                memoryMap;
    UINTN                                 memoryMapSize, mapKey, descriptorSize;
    UINT32                                descriptorVersion;

    EFI_CONFIGURATION_TABLE*              CT = ST->ConfigurationTable;
    EFI_GUID                              acpi20_guid = {0x8868E871, 0xE4F1, 0x11D3, {0xBC, 0x22, 0x00, 0x80, 0xC7, 0x3C, 0x88, 0x81}};

    EFI_GUID                              acpi10_guid = {0xEB9D2D31, 0x2D88, 0x11D3, {0x9A, 0x16, 0x00, 0x90, 0x27, 0x3F, 0xC1, 0x4D}};
    void*                                 rsdp = NULL;

    EFI_STATUS                            status;

    Print(L"\nST (SystemTable): %p, entries: %d\n", ST, ST->NumberOfTableEntries);
    for (UINTN i = 0; i < ST->NumberOfTableEntries; i++) {
        Print(L"[%d] GUID: %g | Table: %p\n",
            i,
            &ST->ConfigurationTable[i].VendorGuid,
            ST->ConfigurationTable[i].VendorTable);
    }
    Print(L"ACPI 2.0: %g\n", &acpi20_guid);
    Print(L"ACPI 1.0: %g\n", &acpi10_guid);

    status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, NULL, (void**)&gop);
    if (EFI_ERROR(status))
    {
        Print(L"error: unable to locate GOP (status: %r)\n", status);
        return status;
    }
    status = uefi_call_wrapper(gop->QueryMode, 4, gop, gop->Mode == NULL ? 0 : gop->Mode->Mode, &gopInfoSize);
    if (status == EFI_NOT_STARTED)
    {
        status = uefi_call_wrapper(gop->SetMode, 2, gop, 0);
    }
    else
    {
        gopNativeMode = gop->Mode->Mode;
        gopModes      = gop->Mode->MaxMode;
    }
    for (int i = 0; i < gopModes; i++)
    {
        status = uefi_call_wrapper(gop->QueryMode, 4, gop, i, &gopInfoSize, &gopInfo);
        if (gopInfo->HorizontalResolution == 1920 && gopInfo->VerticalResolution == 1080)
        {
            Print(L"\nfound: 1920x1080 | mode: %03d\n", i);
            status = uefi_call_wrapper(gop->SetMode, 2, gop, i);
            if (EFI_ERROR(status))
            {
                Print(L"warn: unable to set 1920x1080 resolution\n");
                status = uefi_call_wrapper(gop->SetMode, 2, gop, gopNativeMode);
            }
            break;
        }
        Print(L"|-------------|------------|\n");
        Print(L"| %4dx%4d   | %03d        |\n",
            gopInfo->HorizontalResolution,
            gopInfo->VerticalResolution,
            i
        );
    }
    Print(L"framebuffer: 0x%x | %d | %dx%d | %d\n",
      gop->Mode->FrameBufferBase,
      gop->Mode->FrameBufferSize,
      gop->Mode->Info->HorizontalResolution,
      gop->Mode->Info->VerticalResolution,
      gop->Mode->Info->PixelsPerScanLine
    );

    // stop here. rsdp not found
    for (UINTN i = 0; i < ST->NumberOfTableEntries; i++)
    {
        Print(L"[ACPI] [%d] VendorGUID: %g\n", i, &CT[i].VendorGuid);
        if (CompareGuid(&CT[i].VendorGuid, &acpi20_guid) ||
            CompareGuid(&CT[i].VendorGuid, &acpi10_guid))
        {
            rsdp = CT[i].VendorTable;
            Print(L"[ACPI] RSDP: %p\n", rsdp);
            break;
        }
    }
    static boot_info boot_info;
    if (rsdp != NULL)
    {
        Print(L"[ACPI] RSDP found: %p\n", rsdp);
        boot_info.rsdp_addr = rsdp;
    }
    else
    {
        Print(L"[ACPI] RSDP not found\n");
        boot_info.rsdp_addr = NULL;
    }

    uefi_call_wrapper(BS->GetMemoryMap, 5, &memoryMapSize, memoryMap, &mapKey, &descriptorSize, &descriptorVersion);

    status = uefi_call_wrapper(BS->ExitBootServices, 2, ImageHandle, mapKey);
    if (EFI_ERROR(status))
    {
        Print(L"error: EBS (status: %r)\n\n", status);
        return status;
    }

    while (1);
}