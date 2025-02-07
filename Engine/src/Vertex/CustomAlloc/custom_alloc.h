#ifndef CUSTOM_ALLOC_H
#define CUSTOM_ALLOC_H

#include <stddef.h>
#include <Vertex/Core/PlatformDetection.h>

#ifdef __cplusplus

#endif

    // Function prototypes
VERTEX_API void* custom_malloc(size_t size, const char* file, int line);
VERTEX_API void* custom_realloc(void* ptr, size_t new_size, const char* file, int line);
VERTEX_API void  custom_free(void* ptr);
VERTEX_API void  custom_allocator_print_stats();
VERTEX_API void* custom_memcpy(void* _Dst, void const* _Src, size_t _Size, const char* file, int line);

    // Macro replacements for tracking
#ifndef CUSTOM_ALLOC_DISABLE_MACROS
#define malloc(size) custom_malloc(size, __FILE__, __LINE__)
#define realloc(ptr, size) custom_realloc(ptr, size, __FILE__, __LINE__)
#define free(ptr) custom_free(ptr)
#define memcpy(Dst, Src, Size)  custom_memcpy(Dst, Src, Size, __FILE__, __LINE__)
#endif

#ifdef __cplusplus

#endif

#endif // CUSTOM_ALLOC_H

