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

    // Macro replacements for tracking
#ifndef CUSTOM_ALLOC_DISABLE_MACROS
#define malloc(size) custom_malloc(size, __FILE__, __LINE__)
#define realloc(ptr, size) custom_realloc(ptr, size, __FILE__, __LINE__)
#define free(ptr) custom_free(ptr)
#endif

#ifdef __cplusplus

#endif

#endif // CUSTOM_ALLOC_H

