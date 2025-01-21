#include "vxpch.h"

#define STB_IMAGE_IMPLEMENTATION

#define STBI_MALLOC(sz)        malloc(sz)
#define STBI_REALLOC(p,newsz)  realloc(p,newsz)
#define STBI_FREE(p)           free(p)

#define STBI_MEMMOVE(a,b,sz) memmove(a,b,sz)

#define STBI_ASSERT(x) VX_CORE_ASSERT(x)
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBIW_MALLOC(sz)        malloc(sz)
#define STBIW_REALLOC(p,newsz)  realloc(p,newsz)
#define STBIW_FREE(p)           free(p)

#define STBIW_MEMMOVE(a,b,sz) memmove(a,b,sz)
#define STBIW_ASSERT(x) VX_CORE_ASSERT(x)

#include "stb_image_write.h"