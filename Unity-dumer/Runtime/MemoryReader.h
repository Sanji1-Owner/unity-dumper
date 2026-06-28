#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <memory>
#include <type_traits>

namespace UnityAnalyzer::Runtime {

class IMemoryReader {
public:
    virtual ~IMemoryReader() = default;
    
    virtual bool IsValid() const = 0;
    virtual bool Read(uintptr_t address, void* buffer, size_t size) = 0;
    
    template<typename T>
    bool SafeRead(uintptr_t address, T& value) {
        static_assert(std::is_trivially_copyable<T>::value, "T must be trivially copyable");
        return Read(address, &value, sizeof(T));
    }

    template<typename T>
    T Read(uintptr_t address) {
        T value{};
        Read(address, &value, sizeof(T));
        return value;
    }

    virtual uintptr_t GetModuleBase(const std::string& module_name) = 0;
    virtual bool IsAddressValid(uintptr_t address) = 0;

    std::string ReadString(uintptr_t address, size_t max_length = 256) {
        if (!IsAddressValid(address)) return "";
        std::vector<char> buffer(max_length);
        if (Read(address, buffer.data(), max_length)) {
            buffer[max_length - 1] = '\0';
            return std::string(buffer.data());
        }
        return "";
    }
};

class LocalMemoryReader : public IMemoryReader {
public:
    bool IsValid() const override { return true; }
    bool Read(uintptr_t address, void* buffer, size_t size) override;
    uintptr_t GetModuleBase(const std::string& module_name) override;
    bool IsAddressValid(uintptr_t address) override;
};

#ifdef _WIN32
class WindowsProcessMemoryReader : public IMemoryReader {
public:
    WindowsProcessMemoryReader(uint32_t process_id);
    ~WindowsProcessMemoryReader();
    bool IsValid() const override { return process_handle_ != nullptr; }
    bool Read(uintptr_t address, void* buffer, size_t size) override;
    uintptr_t GetModuleBase(const std::string& module_name) override;
    bool IsAddressValid(uintptr_t address) override;
private:
    void* process_handle_;
    uint32_t pid_;
};
#endif

#if defined(__linux__) || defined(__ANDROID__)
class LinuxProcessMemoryReader : public IMemoryReader {
public:
    LinuxProcessMemoryReader(uint32_t process_id);
    bool IsValid() const override { return pid_ != 0; }
    bool Read(uintptr_t address, void* buffer, size_t size) override;
    uintptr_t GetModuleBase(const std::string& module_name) override;
    bool IsAddressValid(uintptr_t address) override;
private:
    uint32_t pid_;
};
#endif

} // namespace UnityAnalyzer::Runtime
