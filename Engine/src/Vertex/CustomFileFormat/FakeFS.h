#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include "Vertex/Core/Base.h"
#include <zlib.h>
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

        std::vector<uint8_t> compressData(const std::vector<uint8_t>& data) {
            uLongf compressedSize = compressBound(data.size());
            std::vector<uint8_t> compressedData(compressedSize);

            if (compress(compressedData.data(), &compressedSize, data.data(), data.size()) != Z_OK) {
                throw std::runtime_error("Failed to compress data.");
            }

            compressedData.resize(compressedSize); // Adjust size to actual compressed data
            return compressedData;
        }

        std::vector<uint8_t> decompressData(const std::vector<uint8_t>& compressedData, uLongf originalSize) {
            std::vector<uint8_t> decompressedData(originalSize);

            if (uncompress(decompressedData.data(), &originalSize, compressedData.data(), compressedData.size()) != Z_OK) {
                throw std::runtime_error("Failed to decompress data.");
            }

            return decompressedData;
        }

        std::vector<uint8_t> encrypt(const std::string& data) const {
            std::vector<uint8_t> encrypted(data.begin(), data.end());
            for (auto& byte : encrypted) {
                byte ^= encryptionKey[0]; // Simple XOR encryption
            }
            return encrypted;
        }

        std::string decrypt(const std::vector<uint8_t>& data) const {
            std::vector<uint8_t> decrypted = data;
            for (auto& byte : decrypted) {
                byte ^= encryptionKey[0]; // Simple XOR decryption
            }
            return std::string(decrypted.begin(), decrypted.end());
        }

        void writeRIFFHeader(std::ofstream& file) const {
            // Example RIFF header
            file.write("RIFF", 4);
            uint32_t size = 0; // Placeholder for size
            file.write(reinterpret_cast<const char*>(&size), 4);
            file.write("FAKE", 4); // Format
            file.write(reinterpret_cast<const char*>(&useCompression), 1); // Write useCompression flag
        }

        void writeRIFFChunk(std::ofstream& file, const std::string& chunkName, const std::vector<uint8_t>& data) const {
            std::string chunkNamePadded = chunkName;
            chunkNamePadded.resize(4, '\0'); // Ensure chunkName is exactly 4 bytes

            file.write(chunkNamePadded.c_str(), 4); // Chunk name
            uint32_t size = data.size();
            file.write(reinterpret_cast<const char*>(&size), 4);
            file.write(reinterpret_cast<const char*>(data.data()), size);

            if (useChecksum) {
                uint32_t checksum = customChecksum(data);
                file.write(reinterpret_cast<const char*>(&checksum), 4);
            }
        }

        void writeRIFFFooter(std::ofstream& file) const {
            // Placeholder for any footer data if needed
        }

        uLongf decryptSize(const std::vector<uint8_t>& compressedData) const {
            uLongf originalSize;
            memcpy(&originalSize, compressedData.data(), sizeof(uLongf));
            return originalSize;
        }
    };

    
}