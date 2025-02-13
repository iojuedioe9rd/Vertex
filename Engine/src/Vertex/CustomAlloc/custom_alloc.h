#ifndef CUSTOM_ALLOC_H
#define CUSTOM_ALLOC_H

#include <stddef.h>
#include <Vertex/Core/PlatformDetection.h>



    // Function prototypes for custom memory management
    VERTEX_API void* custom_malloc(size_t size, const char* file, int line);
    VERTEX_API void* custom_realloc(void* ptr, size_t new_size, const char* file, int line);
    VERTEX_API void  custom_free(void* ptr);
    VERTEX_API void  custom_allocator_print_stats();
    VERTEX_API void* custom_memcpy(void* _Dst, void const* _Src, size_t _Size, const char* file, int line);
#define CUSTOM_ALLOC_DISABLE_MACROS
#ifndef CUSTOM_ALLOC_DISABLE_MACROS
#define malloc(size) custom_malloc(size, __FILE__, __LINE__)
#define realloc(ptr, size) custom_realloc(ptr, size, __FILE__, __LINE__)
#define free(ptr) custom_free(ptr)
#define memcpy(Dst, Src, Size) custom_memcpy(Dst, Src, Size, __FILE__, __LINE__)
#endif


/*
#ifdef __cplusplus
// Now, with C++ linkage, declare the global new and delete operators.
// Temporarily disable any macros for new/delete (if defined by MSVC).
#pragma push_macro("new")
#pragma push_macro("delete")
#undef new
#undef delete

// Global operator new and operator new[] declarations:
void* operator new(size_t size);
void* operator new[](size_t size);
void* operator new(size_t size, const char* file, int line);
void* operator new[](size_t size, const char* file, int line);

// Global operator delete and operator delete[] declarations:
void operator delete(void* ptr) noexcept;
void operator delete[](void* ptr) noexcept;
void operator delete(void* ptr, size_t size) noexcept;
void operator delete[](void* ptr, size_t size) noexcept;

#pragma pop_macro("new")
#pragma pop_macro("delete")
#endif*/

#endif // CUSTOM_ALLOC_H
