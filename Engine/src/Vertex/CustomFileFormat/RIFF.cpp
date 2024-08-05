#include "vxpch.h"
#include "RIFF.h"
#include "XOR.h"
#include "compress.h"
#include <Checksum.h>

#define RIFF_VERSION 1
namespace Vertex
{

    VERTEX_API void writeRIFFFile(const std::string& filename, const FakeFS& fakefs, const std::string& encryptionKey) {
        RIFFHeader riffHeader;
        strncpy(riffHeader.chunkID, "RIFF", 4);
        strncpy(riffHeader.format, "VTXA", 4);
        riffHeader.useMono = 0;
        riffHeader.useXOR = 1;
        riffHeader.XORKey = 1234;

        riffHeader.version = RIFF_VERSION;


        std::ofstream outFile(filename, std::ios::binary);
        if (!outFile) {
            std::cerr << "Failed to open file for writing: " << filename << std::endl;
            return;
        }

        outFile.write(reinterpret_cast<char*>(&riffHeader), sizeof(riffHeader));

        uint32_t riffSize = 4;

        for (const auto& entry : fakefs.files) {
            const std::string& path = entry.first;
            const File& file = entry.second;

            Chunk chunk;
            strncpy(chunk.id, file.name.c_str(), 4);
            chunk.path = path;
            std::string dataToCompress = path + '\0' + file.content;
            std::string compressedData = compressString(dataToCompress);
            std::string encryptedData = xorEncryptDecrypt(compressedData, encryptionKey);
            chunk.size = static_cast<uint32_t>(encryptedData.size());
            chunk.data = new char[chunk.size];
            memcpy(chunk.data, encryptedData.data(), chunk.size);
            chunk.compressed = true;

            std::vector<uint8_t> chunkData(chunk.data, chunk.data + chunk.size);
            chunk.checksum = customChecksum(chunkData);

            outFile.write(chunk.id, 4);
            outFile.write(reinterpret_cast<char*>(&chunk.size), sizeof(chunk.size));
            outFile.write(reinterpret_cast<char*>(&chunk.compressed), sizeof(chunk.compressed));
            outFile.write(reinterpret_cast<char*>(&chunk.checksum), sizeof(chunk.checksum));
            outFile.write(chunk.data, chunk.size);

            std::cout << "Written chunk: " << std::string(chunk.id, 4)
                << ", size: " << chunk.size
                << ", checksum: " << chunk.checksum << std::endl;

            riffSize += 12 + sizeof(chunk.compressed) + chunk.size;
        }

        riffHeader.chunkSize = riffSize;
        outFile.seekp(0, std::ios::beg);
        outFile.write(reinterpret_cast<char*>(&riffHeader), sizeof(riffHeader));


        outFile.close();
        std::cout << "RIFF file written successfully: " << filename << std::endl;
    }

    VERTEX_API void readRIFFFile(const std::string& filename, FakeFS& fakefs, const std::string& encryptionKey) {
        std::ifstream inFile(filename, std::ios::binary);
        if (!inFile) {
            throw std::runtime_error("Failed to open file for reading: " + filename);
        }

        RIFFHeader riffHeader;
        inFile.read(reinterpret_cast<char*>(&riffHeader), sizeof(riffHeader));

        if (strncmp(riffHeader.chunkID, "RIFF", 4) != 0 || strncmp(riffHeader.format, "VTXA", 4) != 0) {
            throw std::runtime_error("Not a valid RIFF file or not in VTXA format.");
        }

        if (riffHeader.version != RIFF_VERSION)
        {
            throw std::runtime_error("Not the VERSION");

        }

        while (inFile.peek() != EOF) {
            Chunk chunk;
            inFile.read(chunk.id, 4);
            inFile.read(reinterpret_cast<char*>(&chunk.size), sizeof(chunk.size));
            inFile.read(reinterpret_cast<char*>(&chunk.compressed), sizeof(chunk.compressed));
            inFile.read(reinterpret_cast<char*>(&chunk.checksum), sizeof(chunk.checksum));
            chunk.data = new char[chunk.size];
            inFile.read(chunk.data, chunk.size);

            std::vector<uint8_t> chunkData(chunk.data, chunk.data + chunk.size);
            uint32_t calculatedChecksum = customChecksum(chunkData);
            if (calculatedChecksum != chunk.checksum) {
                throw std::runtime_error("Checksum mismatch for chunk: " + std::string(chunk.id, 4));
            }

            std::string encryptedData(chunk.data, chunk.size);
            std::string compressedData = xorEncryptDecrypt(encryptedData, encryptionKey);
            std::string dataStr = decompressString(compressedData);

            size_t pathEnd = dataStr.find('\0');
            chunk.path = dataStr.substr(0, pathEnd);
            std::string content = dataStr.substr(pathEnd + 1);


            fakefs.addFile(chunk.path, File(chunk.id, content));
        }

        inFile.close();
    }
}