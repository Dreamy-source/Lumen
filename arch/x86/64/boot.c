#include <efi/efi.h>
#include <efi/efilib.h>

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
    InitializeLib(ImageHandle, SystemTable);

    Print(L"Welcome to Lumen\r\n");

    return EFI_SUCCESS;
}