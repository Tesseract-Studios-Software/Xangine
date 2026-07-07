#pragma once

#include <cstdlib>
#include <cstdio>

namespace Xangine {

inline unsigned char* stbi_load(const char*, int* x, int* y, int* channels_in_file, int) {
    if (x) *x = 0;
    if (y) *y = 0;
    if (channels_in_file) *channels_in_file = 0;
    return nullptr;
}

inline unsigned char* stbi_load_from_memory(const unsigned char*, int, int* x, int* y, int* channels_in_file, int) {
    if (x) *x = 0;
    if (y) *y = 0;
    if (channels_in_file) *channels_in_file = 0;
    return nullptr;
}

inline void stbi_image_free(void* ptr) {
    std::free(ptr);
}

} // namespace Xangine

using Xangine::stbi_load;
using Xangine::stbi_load_from_memory;
using Xangine::stbi_image_free;
