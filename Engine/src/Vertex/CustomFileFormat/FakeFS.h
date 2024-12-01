#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include "Vertex/Core/Base.h"
#include "../../../vendor/checksum/src/Checksum.h"
#include "../Core/Logger.h"
#include <filesystem>



namespace Vertex
{
    // Define constants for chunk types
    const char RIFF_ID[] = "RIFF";
    const char VERT_ID[] = "VERT"; // Custom format identifier for Vertex
    const char FILE_ID[] = "FILE";
    const char COMPRESSED_ID[] = "COMP";

    // File metadata structure
    struct FileMetadata {
        std::string name;
        size_t size;
    };

    class VERTEX_API FakeFS {
    public:
        FakeFS(const std::string& encryptionKey, bool useChecksum = false)
            : encryptionKey(encryptionKey), useChecksum(useChecksum) {}

        bool createFile(const std::string& filename, const std::string& content);

        bool updateFile(const std::string& filename, const std::string& content);

        std::string readFile(const std::string& filename);

        bool deleteFile(const std::string& filename);
         
        void listFiles() const;

        bool createRIFFFromDirectory(const std::string& folderPath, const std::string& riffFilePath, bool compress);

        bool writeRIFFFile(const std::string& riffFilePath);

        bool readRIFFFile(const std::string& riffFilePath);

    private:
        std::unordered_map<std::string, std::vector<uint8_t>> files;
        std::string encryptionKey;
        bool useChecksum;
        bool useCompression = 1;

        std::vector<uint8_t> compressData(const std::vector<uint8_t>& data);

        std::vector<uint8_t> decompressData(const std::vector<uint8_t>& compressedData, unsigned long originalSize);

        std::vector<uint8_t> encrypt(const std::string& data) const;

        std::string decrypt(const std::vector<uint8_t>& data) const;

        void writeRIFFHeader(std::ofstream& file) const;

        void writeRIFFChunk(std::ofstream& file, const std::string& chunkName, const std::vector<uint8_t>& data) const;

        void writeRIFFFooter(std::ofstream& file) const {
            // Placeholder for any footer data if needed
        }

        unsigned long decryptSize(const std::vector<uint8_t>& compressedData) const;
    };

    
}