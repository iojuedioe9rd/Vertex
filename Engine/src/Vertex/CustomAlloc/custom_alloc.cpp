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
#ifndef VX_DEBUG
    size_t total_size = sizeof(MemBlock) + size + ALIGNMENT;
    void* raw_ptr = malloc(total_size);
    if (!raw_ptr) return nullptr;

    void* aligned_ptr = (void*)(((uintptr_t)raw_ptr + ALIGNMENT - 1) & ~(ALIGNMENT - 1));
    MemBlock* block = (MemBlock*)aligned_ptr - 1;

    block->size = size;
    block->file = file;
    block->line = line;
    block->timestamp = std::time(nullptr);
    block->canary_pre = CANARY_VALUE;
    block->canary_post = CANARY_VALUE;

    thread_memory_info.allocations[aligned_ptr] = *block;
    thread_memory_info.total_allocated += size;

    if (thread_memory_info.total_allocated > thread_memory_info.peak_allocated)
        thread_memory_info.peak_allocated = thread_memory_info.total_allocated;

    global_allocated += size;
    size_t peak = global_allocated.load();
    if (peak > global_peak_allocated.load())
        global_peak_allocated.store(peak);

    if (has_fma3)
        fma3_memset(aligned_ptr, size);
    else if (has_sse4)
        sse4_memset(aligned_ptr, size);
    else if (has_xop)
        xop_memset(aligned_ptr, size);
    else
        sse2_memset(aligned_ptr, size);

    //log_memory_op("MALLOC", aligned_ptr, size, file, line);

#else
	void* aligned_ptr = std::malloc(size);
	if (!aligned_ptr) return nullptr;
#endif


    return aligned_ptr;
}

// FMA3/SSE4/SSE2/XOP-aligned realloc
void* custom_realloc(void* ptr, size_t new_size, const char* file, int line) {
    if (!ptr) return custom_malloc(new_size, file, line);
#ifndef VX_DEBUG
    auto it = thread_memory_info.allocations.find(ptr);
    if (it == thread_memory_info.allocations.end()) {
        std::cerr << "[ERROR] Attempted to realloc an unknown pointer!" << std::endl;
        return nullptr;
    }

    size_t old_size = it->second.size;
    void* new_ptr = custom_malloc(new_size, file, line);
    if (!new_ptr) return nullptr;

    if (has_fma3)
        fma3_memcpy(new_ptr, ptr, (old_size < new_size) ? old_size : new_size);
    else if (has_sse4)
        sse4_memcpy(new_ptr, ptr, (old_size < new_size) ? old_size : new_size);
    else if (has_xop)
        xop_memcpy(new_ptr, ptr, (old_size < new_size) ? old_size : new_size);
    else
        sse2_memcpy(new_ptr, ptr, (old_size < new_size) ? old_size : new_size);

    custom_free(ptr);
    log_memory_op("REALLOC", new_ptr, new_size, file, line);

#else
	void* new_ptr = std::realloc(ptr, new_size);
	if (!new_ptr) return nullptr;
#endif
    return new_ptr;
}

void custom_free(void* ptr) {
#ifndef VX_DEBUG
    if (!ptr) return;  // Don't free null pointers




    // Find the memory block information from the thread-local memory map
    auto it = thread_memory_info.allocations.find(ptr);
    if (it == thread_memory_info.allocations.end()) {
        std::cerr << "[ERROR] Attempted to free an unknown or invalid pointer!" << std::endl;
        return;
    }



    MemBlock* block = &it->second;

    // Validate memory integrity using canary values
    if (block->canary_pre != CANARY_VALUE || block->canary_post != CANARY_VALUE) {
        std::cerr << "[ERROR] Buffer overflow detected while freeing memory!" << std::endl;
        return;
    }

    // Log the free operation with file/line details
    log_memory_op("FREE", ptr, block->size, block->file, block->line);

    // Update thread-local and global memory stats
    thread_memory_info.total_allocated -= block->size;
    global_allocated -= block->size;

    

    // Free the memory
	IMPL_FUNC(memset, ptr, block->size);

#else
	std::free(ptr);

#endif // !VX_DEBUG
    
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
