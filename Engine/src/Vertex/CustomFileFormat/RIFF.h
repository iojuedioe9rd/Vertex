#pragma once
#include <cstdint>
#include <string>
#include "FakeFS.h"
#include "Vertex/Core.h"
namespace Vertex
{
    struct RIFFHeader {
        char chunkID[4];    // 'RIFF'
        uint32_t chunkSize; // File size - 8 bytes
        char format[4];     // 'VTXA' for Vertex Asset Bundle
        bool useMono;       // TODO: Mono
        bool useXOR;        // XOR
        uint32_t XORKey;    // XOR Key
        uint32_t version;   // Version of the asset bundle format

    };

    // Define a chunk structure with path and compression flag
    struct Chunk {
        char id[4];
        uint32_t size;
        std::string path;
        char* data;
        bool compressed;

        Chunk() : data(nullptr), compressed(false) {}
        ~Chunk() { delete[] data; }
    };
    VERTEX_API void writeRIFFFile(const std::string& filename, const FakeFS& fakefs, const std::string& encryptionKey);
    VERTEX_API void readRIFFFile(const std::string& filename, FakeFS& fakefs, const std::string& encryptionKey);
}