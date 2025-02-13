#include <vxpch.h>

#define CUSTOM_ALLOC_IMPLEMENTATION
#define CUSTOM_ALLOC_DISABLE_MACROS


#ifdef CUSTOM_ALLOC_IMPLEMENTATION

#include "custom_alloc.h"
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <thread>
#include <atomic>
#include <unordered_map>
#include <mutex>
#include <immintrin.h>  // SSE2, SSE4, XOP, FMA3
#include <intrin.h>


#define CANARY_VALUE 0xDEADC0DE
#define HASH_TABLE_SIZE 256
#define ALIGNMENT 16  // SSE4/SSE2/XOP/FMA3 prefer 16-byte alignment

// Runtime feature detection
static bool has_sse4 = false;
static bool has_xop = false;
static bool has_fma3 = false;
/*
void* operator new(size_t size) {
    return custom_malloc(size, "Unknown", 0);
}

void* operator new[](size_t size) {
    return custom_malloc(size, "Unknown", 0);
}

void* operator new(size_t size, const char* file, int line) {
    return custom_malloc(size, file, line);
}

void* operator new[](size_t size, const char* file, int line) {
    return custom_malloc(size, file, line);
}

void operator delete(void* ptr) noexcept {
    custom_free(ptr);
}

void operator delete[](void* ptr) noexcept {
    custom_free(ptr);
}

void operator delete(void* ptr, size_t size) noexcept {
    (void)size; // Size is not used in custom_free
    custom_free(ptr);
}

void operator delete[](void* ptr, size_t size) noexcept {
    (void)size;
    custom_free(ptr);
}*/

void detect_features() {
    int cpu_info[4] = { 0 };
    __cpuid(cpu_info, 0x80000001);
    has_xop = (cpu_info[2] & (1 << 11));  // Bit 11 in ECX indicates XOP support

    __cpuid(cpu_info, 0);
    has_sse4 = (cpu_info[3] & (1 << 19)) != 0;  // Bit 19 in ECX indicates SSE4.1 support
    has_fma3 = (cpu_info[2] & (1 << 12)) != 0; // Bit 12 in ECX indicates FMA3 support

    
}

// Memory block structure
struct MemBlock {
    void* raw_ptr;    // Pointer returned by malloc (needed for free)
    size_t size;
    const char* file;
    int line;
    time_t timestamp;
    uint32_t canary_pre;
    uint32_t canary_post;
};

// Thread-local memory tracking
struct ThreadMemoryInfo {
    std::unordered_map<void*, MemBlock> allocations;
    size_t total_allocated = 0;
    size_t peak_allocated = 0;
};

// Global tracking
static thread_local ThreadMemoryInfo thread_memory_info;
static std::atomic<size_t> global_allocated(0);
static std::atomic<size_t> global_peak_allocated(0);
static std::mutex global_lock;

static inline void fma3_memset(void* ptr, size_t size) {
    __m128i zero = _mm_setzero_si128();
    char* p = (char*)ptr;

    while (size >= 64) {
        _mm_store_si128((__m128i*)p, zero);
        _mm_store_si128((__m128i*)(p + 16), zero);
        _mm_store_si128((__m128i*)(p + 32), zero);
        _mm_store_si128((__m128i*)(p + 48), zero);
        p += 64;
        size -= 64;
    }
    while (size >= 16) {
        _mm_store_si128((__m128i*)p, zero);
        p += 16;
        size -= 16;
    }
    while (size--) *p++ = 0;
}

// XOP optimized memory zeroing (only for AMD processors with XOP support)
static inline void xop_memset(void* ptr, size_t size) {
    // XOP-specific operations (not supported on all CPUs)
    __m128i zero = _mm_setzero_si128();
    char* p = (char*)ptr;

    while (size >= 32) {
        _mm_store_si128((__m128i*)p, zero);
        _mm_store_si128((__m128i*)(p + 16), zero);
        p += 32;
        size -= 32;
    }
    while (size >= 16) {
        _mm_store_si128((__m128i*)p, zero);
        p += 16;
        size -= 16;
    }
    while (size--) *p++ = 0;
}

// SSE4 optimized memory zeroing
static inline void sse4_memset(void* ptr, size_t size) {
    __m128i zero = _mm_setzero_si128();
    char* p = (char*)ptr;

    while (size >= 64) {
        _mm_store_si128((__m128i*)p, zero);
        _mm_store_si128((__m128i*)(p + 16), zero);
        _mm_store_si128((__m128i*)(p + 32), zero);
        _mm_store_si128((__m128i*)(p + 48), zero);
        p += 64;
        size -= 64;
    }
    while (size >= 16) {
        _mm_store_si128((__m128i*)p, zero);
        p += 16;
        size -= 16;
    }
    while (size--) *p++ = 0;
}

// SSE2 optimized memory zeroing
static inline void sse2_memset(void* ptr, size_t size) {
    __m128i zero = _mm_setzero_si128();
    char* p = (char*)ptr;

    while (size >= 16) {
        _mm_store_si128((__m128i*)p, zero);
        p += 16;
        size -= 16;
    }
    while (size--) *p++ = 0;
}

static inline void* xop_memcpy(void* dst, const void* src, size_t size) {
    __m128i* d = (__m128i*)dst;
    const __m128i* s = (const __m128i*)src;

    while (size >= 32) {
        _mm_store_si128(d++, _mm_load_si128(s++));
        _mm_store_si128(d++, _mm_load_si128(s++));
        size -= 32;
    }
    return std::memcpy(d, s, size);
}

// FMA3 optimized memory copy (if supported)
static inline void* fma3_memcpy(void* dst, const void* src, size_t size) {
    __m128i* d = (__m128i*)dst;
    const __m128i* s = (const __m128i*)src;

    while (size >= 64) {
        _mm_store_si128(d++, _mm_load_si128(s++));
        _mm_store_si128(d++, _mm_load_si128(s++));
        _mm_store_si128(d++, _mm_load_si128(s++));
        _mm_store_si128(d++, _mm_load_si128(s++));
        size -= 64;
    }
    while (size >= 16) {
        _mm_store_si128(d++, _mm_load_si128(s++));
        size -= 16;
    }
    return std::memcpy(d, s, size);
}

// SSE4 optimized memory copy
static inline void* sse4_memcpy(void* dst, const void* src, size_t size) {
    __m128i* d = (__m128i*)dst;
    const __m128i* s = (const __m128i*)src;

    while (size >= 64) {
        _mm_store_si128(d++, _mm_load_si128(s++));
        _mm_store_si128(d++, _mm_load_si128(s++));
        _mm_store_si128(d++, _mm_load_si128(s++));
        _mm_store_si128(d++, _mm_load_si128(s++));
        size -= 64;
    }
    while (size >= 16) {
        _mm_store_si128(d++, _mm_load_si128(s++));
        size -= 16;
    }
    return std::memcpy(d, s, size);
}

// SSE2 optimized memory copy
static inline void* sse2_memcpy(void* dst, const void* src, size_t size) {
    __m128i* d = (__m128i*)dst;
    const __m128i* s = (const __m128i*)src;

    while (size >= 16) {
        _mm_store_si128(d++, _mm_load_si128(s++));
        size -= 16;
    }
    return std::memcpy(d, s, size);
}

// Logging utility
static void log_memory_op(const char* op, void* ptr, size_t size, const char* file, int line) {
    std::lock_guard<std::mutex> lock(global_lock);
    std::cout << "[" << std::time(nullptr) << "] " << op << ": " << ptr
        << " (" << size << " bytes) at " << file << ":" << line << std::endl;
}

#define IMPL_FUNC(func, ptr, size) \
    if (has_fma3) \
        fma3_##func(ptr, size); \
    else if (has_sse4) \
        sse4_##func(ptr, size); \
    else if (has_xop) \
        xop_##func(ptr, size); \
    else \
        sse2_##func(ptr, size)

// FMA3/SSE4/SSE2/XOP-aligned malloc
void* custom_malloc(size_t size, const char* file, int line) {

    return std::malloc(size);

    // Total size: header + requested size + extra for alignment adjustment.
    size_t total_size = sizeof(MemBlock) + size + ALIGNMENT;
    void* raw_ptr = std::malloc(total_size);
    if (!raw_ptr)
        return nullptr;

    // Compute an aligned address that leaves space for the header.
    uintptr_t raw_addr = reinterpret_cast<uintptr_t>(raw_ptr);
    uintptr_t aligned_addr = (raw_addr + sizeof(MemBlock) + ALIGNMENT - 1) & ~(ALIGNMENT - 1);
    void* aligned_ptr = reinterpret_cast<void*>(aligned_addr);

    // Store the header immediately before the aligned pointer.
    MemBlock* block = reinterpret_cast<MemBlock*>(aligned_ptr) - 1;
    block->raw_ptr = raw_ptr;  // Save original pointer to free() later.
    if (raw_ptr == (void*)0x000002877f5e6c30)
    {
        __debugbreak();
    }
    block->size = size;
    block->file = file;
    block->line = line;
    block->timestamp = std::time(nullptr);
    block->canary_pre = CANARY_VALUE;
    block->canary_post = CANARY_VALUE;

    // Zero out the allocated memory.
    // (Replace these with your optimized SIMD versions if available.)
    if (has_fma3)
        fma3_memset(aligned_ptr, size);
    else if (has_sse4)
        sse4_memset(aligned_ptr, size);
    else if (has_xop)
        xop_memset(aligned_ptr, size);
    else
        sse2_memset(aligned_ptr, size);

    return aligned_ptr;

    // In debug mode, simply call std::malloc.
    //return std::malloc(size);

}


// FMA3/SSE4/SSE2/XOP-aligned realloc
void* custom_realloc(void* ptr, size_t new_size, const char* file, int line) {

    return realloc(ptr, new_size);

    // If the pointer is null, behave like custom_malloc.
    if (!ptr)
        return custom_malloc(new_size, file, line);

    // Retrieve the header. The header was stored immediately before the aligned pointer.
    MemBlock* old_block = reinterpret_cast<MemBlock*>(ptr) - 1;

    // Validate the canary values to detect possible memory corruption.
    if (old_block->canary_pre != CANARY_VALUE || old_block->canary_post != CANARY_VALUE) {
        std::cerr << "[ERROR] Buffer overflow detected in custom_realloc for memory at " << ptr << std::endl;
        return nullptr;
    }

    // Get the old allocation size.
    size_t old_size = old_block->size;

    // Allocate a new memory block with the new size.
    void* new_ptr = custom_malloc(new_size, file, line);
    if (!new_ptr)
        return nullptr;

    // Calculate the number of bytes to copy (the smaller of the two sizes).
    size_t copy_size = (old_size < new_size) ? old_size : new_size;

    // Use your custom_memcpy to copy the data from the old block to the new block.
    // (If you have SIMD optimizations in custom_memcpy, they will be used.)
    custom_memcpy(new_ptr, ptr, copy_size, file, line);

    // Free the old block.
    custom_free(ptr);

    // Return the new pointer.
    return new_ptr;

   // In debug mode, fall back to the standard realloc.
    //return std::realloc(ptr, new_size);

}

void custom_free(void* ptr) {

    return free(ptr);

    if (!ptr)
        return;  // Do nothing if pointer is null.

    // Get the header stored just before the aligned pointer.
    MemBlock* block = reinterpret_cast<MemBlock*>(ptr) - 1;

    // Validate the canary values to detect buffer overruns.
    if (block->canary_pre != CANARY_VALUE || block->canary_post != CANARY_VALUE) {
        std::cerr << "[ERROR] Buffer overflow detected while freeing memory!" << std::endl;
        return;
    }

    // Optionally, you can clear the memory before freeing.
    if (has_fma3)
        fma3_memset(ptr, block->size);
    else if (has_sse4)
        sse4_memset(ptr, block->size);
    else if (has_xop)
        xop_memset(ptr, block->size);
    else
        sse2_memset(ptr, block->size);

    // Free the original pointer returned by malloc.
    std::free(block->raw_ptr);

    //std::free(ptr);
//#endif
}

void custom_allocator_print_stats()
{
	//std::lock_guard<std::mutex> lock(global_lock);
	//std::cout << "Thread-local memory stats:" << std::endl;
	//std::cout << "  Total allocated: " << thread_memory_info.total_allocated << " bytes" << std::endl;
	//std::cout << "  Peak allocated: " << thread_memory_info.peak_allocated << " bytes" << std::endl;
	//std::cout << "Global memory stats:" << std::endl;
	//std::cout << "  Total allocated: " << global_allocated.load() << " bytes" << std::endl;
	//std::cout << "  Peak allocated: " << global_peak_allocated.load() << " bytes" << std::endl;
	//std::cout << "  Active threads: " << std::thread::hardware_concurrency() << std::endl;
}

void* custom_memcpy(void* _Dst, void const* _Src, size_t _Size, const char* file, int line)
{
    if (has_fma3)
        return fma3_memcpy(_Dst, _Src, _Size);
    else if (has_sse4)
        return sse4_memcpy(_Dst, _Src, _Size);
    else if (has_xop)
        return xop_memcpy(_Dst, _Src, _Size);
    else
        return sse2_memcpy(_Dst, _Src, _Size);
}

// Ensure cleanup runs at thread exit
struct Cleanup {
    Cleanup() { detect_features(); }
    ~Cleanup() { custom_allocator_print_stats(); }
};
static thread_local Cleanup cleanup;

#endif // CUSTOM_ALLOC_IMPLEMENTATION
