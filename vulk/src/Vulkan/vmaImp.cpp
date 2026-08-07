#include <iostream>
#include <cstdarg>

#define VMA_DEBUG_LOG(format, ...) vmaCustomLog(format, ##__VA_ARGS__)

inline void vmaCustomLog(const char* format, ...) {
    const char* CYAN = "\033[36m";
    const char* RESET = "\033[0m";

    std::cout << CYAN << "[VMA]: " << RESET;

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    std::cout << std::endl;
}

#define VMA_IMPLEMENTATION
#define VMA_STATIC_VULKAN_FUNCTIONS_0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS_1
#define VMA_VULKAN_VERSION 1004000
#include <VMA/vk_mem_alloc.h>