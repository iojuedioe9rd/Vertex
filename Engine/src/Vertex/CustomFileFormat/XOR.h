#pragma once
#include <string>
#include "Vertex/Core/Base.h"
namespace Vertex
{

    VERTEX_API void xorEncryptDecrypt(char* data, size_t size, const std::string& key) {
        size_t keyLength = key.size();
        for (size_t i = 0; i < size; ++i) {
            data[i] ^= key[i % keyLength];
        }
    }

    VERTEX_API std::string xorEncryptDecrypt(const std::string& data, const std::string& key) {
        std::string result = data;
        xorEncryptDecrypt(&result[0], result.size(), key);
        return result;
    }
}