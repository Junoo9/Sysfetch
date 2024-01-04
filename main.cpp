#include <iostream>
#include <windows.h>

void CpuInformation() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    // Processor architecture
    std::cout << "Processor Architecture: ";
    switch (sysInfo.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_INTEL:
            std::cout << "x86 (32-bit)";
            break;
        case PROCESSOR_ARCHITECTURE_AMD64:
            std::cout << "x64 (64-bit)";
            break;
        case PROCESSOR_ARCHITECTURE_ARM:
            std::cout << "ARM";
            break;
        case PROCESSOR_ARCHITECTURE_ARM64:
            std::cout << "ARM64 (64-bit ARM)";
            break;
        default:
            std::cout << "Unknown";
            break;
    }
    std::cout << std::endl;

    // Number of processors
    std::cout << "Number of Processors: " << sysInfo.dwNumberOfProcessors << std::endl;

    // Number of physical cores
    SYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer[4096];
    DWORD length = sizeof(buffer);
    if (GetLogicalProcessorInformation(buffer, &length)) {
        int coreCount = 0;
        for (DWORD i = 0; i < length / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION); ++i) {
            if (buffer[i].Relationship == RelationProcessorCore) {
                coreCount++;
            }
        }
        std::cout << "Number of Physical Cores: " << coreCount << std::endl;
    }

    // CPU name and clocking speed (remaining code unchanged)
    char cpuName[256];
    DWORD size = sizeof(cpuName);
    if (RegGetValue(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "ProcessorNameString", RRF_RT_ANY, NULL, cpuName, &size) == ERROR_SUCCESS) {
        std::cout << "CPU Name: " << cpuName << std::endl;
    }

    HKEY hKey;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD clockSpeed;
        DWORD dataSize = sizeof(DWORD);
        if (RegQueryValueEx(hKey, "~MHz", NULL, NULL, reinterpret_cast<LPBYTE>(&clockSpeed), &dataSize) == ERROR_SUCCESS) {
            std::cout << "Clocking Speed: " << (float)clockSpeed / 1000 << " GHz" << std::endl;
        }
        RegCloseKey(hKey);
    }
}

void RamInformation() {
    MEMORYSTATUSEX memoryStatus;
    memoryStatus.dwLength = sizeof(memoryStatus);

    if (GlobalMemoryStatusEx(&memoryStatus)) {
        std::cout << "Total Physical Memory: " << memoryStatus.ullTotalPhys / (1024.0f * 1024 * 1024) << " GB" << std::endl;
        std::cout << "Available Physical Memory: " << memoryStatus.ullAvailPhys / (1024.0f * 1024 * 1024) << " GB" << std::endl;
        std::cout << "Total Virtual Memory: " << memoryStatus.ullTotalVirtual / 1024 / 1024 / 1024 / 1024 << " MB" << std::endl;  // Corrected line
        std::cout << "Available Virtual Memory: " << memoryStatus.ullAvailVirtual / 1024 / 1024 << " MB" << std::endl;
    } else {
        std::cerr << "Failed to retrieve memory information." << std::endl;
    }
}

void RomInformation() {
    DWORD drives = GetLogicalDrives();

    for (int drive = 0; drive < 26; ++drive) {
        if ((drives & (1 << drive)) != 0) {
            std::string rootPath = std::string(1, 'A' + drive) + ":\\";
            char volumeName[MAX_PATH + 1];
            char fileSystemName[MAX_PATH + 1];
            DWORD serialNumber;
            DWORD maxComponentLen;
            DWORD fileSystemFlags;

            if (GetVolumeInformation(rootPath.c_str(), volumeName, MAX_PATH + 1, &serialNumber, &maxComponentLen, &fileSystemFlags, fileSystemName, MAX_PATH + 1)) {
                ULARGE_INTEGER totalBytes;
                ULARGE_INTEGER freeBytes;
                ULARGE_INTEGER totalFreeBytes;

                if (GetDiskFreeSpaceEx(rootPath.c_str(), &freeBytes, &totalBytes, &totalFreeBytes)) {
                    std::cout << "Drive: " << rootPath << std::endl;
                    std::cout << "Device Name: " << volumeName << std::endl;
                    std::cout << "Total Storage: " << totalBytes.QuadPart / (1024.0 * 1024 * 1024) << " GB" << std::endl;
                    std::cout << "Storage Available: " << totalFreeBytes.QuadPart / (1024.0 * 1024 * 1024) << " GB" << std::endl;
                    std::cout << std::endl;
                } else {
                    std::cerr << "Failed to retrieve storage information for " << rootPath << std::endl;
                }
            } else {
                std::cerr << "Failed to retrieve volume information for " << rootPath << std::endl;
            }
        }
    }
}

void DisplayGraphicsCardInfo() {
    DISPLAY_DEVICE displayDevice;
    displayDevice.cb = sizeof(DISPLAY_DEVICE);
    int deviceIndex = 0;

    while (EnumDisplayDevices(NULL, deviceIndex, &displayDevice, 0)) {
        DEVMODE devMode;
        devMode.dmSize = sizeof(DEVMODE);

        if (EnumDisplaySettings(displayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &devMode)) {
            if (devMode.dmBitsPerPel != 0 && devMode.dmPelsWidth != 0 && devMode.dmPelsHeight != 0) {
                std::cout << "Device Name: " << displayDevice.DeviceName << std::endl;
                std::cout << "Device String: " << displayDevice.DeviceString << std::endl;
                std::cout << "Resolution: " << devMode.dmPelsWidth << "x" << devMode.dmPelsHeight << std::endl;
                std::cout << "Color Depth: " << devMode.dmBitsPerPel << " bits per pixel" << std::endl;
            }
        }

        deviceIndex++;
    }
}

int main() {
    std::cout << "===========CPU Information===========" << std::endl;
    CpuInformation();
    std::cout << "===========RAM Information===========" << std::endl;
    RamInformation();
    std::cout << "===========ROM Information===========" << std::endl;
    RomInformation();
    std::cout << "===========GPU Information===========" << std::endl;
    DisplayGraphicsCardInfo();
    std::cout << "=====================================" << std::endl;
    system("pause");
    return 0;
}
