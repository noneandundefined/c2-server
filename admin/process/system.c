#include <iphlpapi.h>
#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#include "system.h"

int get_mac_address(uint8_t mac_address[6]) {
    IP_ADAPTER_INFO adapterInfo[16];
    DWORD bufLen = sizeof(adapterInfo);
    DWORD status = GetAdaptersInfo(adapterInfo, &bufLen);

    if (status != ERROR_SUCCESS) {
        printf("GetAdaptersInfo failed with error: %lu\n", status);
        return -1;
    }

    PIP_ADAPTER_INFO adapter = adapterInfo;

    if (adapter->AddressLength != 6) {
        printf("Unexpected MAC address length: %d\n", adapter->AddressLength);
        return -1;
    }

    memcpy(mac_address, adapter->Address, 6);
    return 0;
}
