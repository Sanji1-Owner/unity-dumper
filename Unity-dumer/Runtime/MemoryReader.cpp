#include "MemoryReader.h"
#include <cstring>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#endif

#if defined(__linux__) || defined(__ANDROID__)
#include <sys/uio.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#endif

namespace UnityAnalyzer::Runtime {

bool LocalMemoryReader::Read(uintptr_t address, void* buffer, size_t size) {
    if (address == 0 || buffer == nullptr) return false;
    // Basic range check if possible, otherwise rely on OS protection (risky for local)
    std::memcpy(buffer, reinterpret_cast<void*>(address), size);
    return true;
}

uintptr_t LocalMemoryReader::GetModuleBase(const std::string& module_name) {
#ifdef _WIN32
    return (uintptr_t)GetModuleHandleA(module_name.c_str());
#else
    // Linux/Android implementation via /proc/self/maps
    std::ifstream maps("/proc/self/maps");
    std::string line;
    while (std::getline(maps, line)) {
        if (line.find(module_name) != std::string::npos) {
            uintptr_t start, end;
            char dash;
            std::stringstream ss(line);
            if (ss >> std::hex >> start >> dash >> end) return start;
        }
    }
    return 0;
#endif
}

bool LocalMemoryReader::IsAddressValid(uintptr_t address) {
    return address != 0; // Simple check for local
}

#ifdef _WIN32
WindowsProcessMemoryReader::WindowsProcessMemoryReader(uint32_t process_id) : pid_(process_id) {
    process_handle_ = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, process_id);
}

WindowsProcessMemoryReader::~WindowsProcessMemoryReader() {
    if (process_handle_) CloseHandle(process_handle_);
}

bool WindowsProcessMemoryReader::Read(uintptr_t address, void* buffer, size_t size) {
    if (!process_handle_ || address == 0) return false;
    SIZE_t bytesRead;
    return ReadProcessMemory(process_handle_, reinterpret_cast<LPCVOID>(address), buffer, size, &bytesRead) && bytesRead == size;
}

uintptr_t WindowsProcessMemoryReader::GetModuleBase(const std::string& module_name) {
    if (!process_handle_) return 0;
    HMODULE hMods[1024];
    DWORD cbNeeded;
    if (EnumProcessModules(process_handle_, hMods, sizeof(hMods), &cbNeeded)) {
        for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
            char szModName[MAX_PATH];
            if (GetModuleFileNameExA(process_handle_, hMods[i], szModName, sizeof(szModName))) {
                std::string path(szModName);
                if (path.find(module_name) != std::string::npos) {
                    MODULEINFO mi;
                    if (GetModuleInformation(process_handle_, hMods[i], &mi, sizeof(mi))) {
                        return (uintptr_t)mi.lpBaseOfDll;
                    }
                }
            }
        }
    }
    return 0;
}

bool WindowsProcessMemoryReader::IsAddressValid(uintptr_t address) {
    if (!process_handle_ || address == 0) return false;
    MEMORY_BASIC_INFORMATION mbi;
    if (VirtualQueryEx(process_handle_, (LPCVOID)address, &mbi, sizeof(mbi))) {
        return (mbi.State == MEM_COMMIT && (mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE)));
    }
    return false;
}
#endif

#if defined(__linux__) || defined(__ANDROID__)
LinuxProcessMemoryReader::LinuxProcessMemoryReader(uint32_t process_id) : pid_(process_id) {}

bool LinuxProcessMemoryReader::Read(uintptr_t address, void* buffer, size_t size) {
    if (pid_ == 0 || address == 0) return false;
    struct iovec local[1];
    struct iovec remote[1];
    local[0].iov_base = buffer;
    local[0].iov_len = size;
    remote[0].iov_base = reinterpret_cast<void*>(address);
    remote[0].iov_len = size;
    ssize_t nread = process_vm_readv(pid_, local, 1, remote, 1, 0);
    return nread == (ssize_t)size;
}

uintptr_t LinuxProcessMemoryReader::GetModuleBase(const std::string& module_name) {
    std::string maps_path = "/proc/" + std::to_string(pid_) + "/maps";
    std::ifstream maps(maps_path);
    std::string line;
    while (std::getline(maps, line)) {
        if (line.find(module_name) != std::string::npos) {
            uintptr_t start, end;
            char dash;
            std::stringstream ss(line);
            if (ss >> std::hex >> start >> dash >> end) return start;
        }
    }
    return 0;
}

bool LinuxProcessMemoryReader::IsAddressValid(uintptr_t address) {
    if (pid_ == 0 || address == 0) return false;
    std::string maps_path = "/proc/" + std::to_string(pid_) + "/maps";
    std::ifstream maps(maps_path);
    std::string line;
    while (std::getline(maps, line)) {
        uintptr_t start, end;
        char dash;
        std::stringstream ss(line);
        if (ss >> std::hex >> start >> dash >> end) {
            if (address >= start && address < end) return true;
        }
    }
    return false;
}
#endif

} // namespace UnityAnalyzer::Runtime
