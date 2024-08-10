#include "vxpch.h"
#include "FakeFS.h"



namespace Vertex
{
    bool FakeFS::createFile(const std::string& filename, const std::string& content) {
        if (files.find(filename) != files.end()) {
            return false; // File already exists
        }
        auto encrypted = encrypt(content);
        if (useCompression) {
            encrypted = compressData(encrypted);
        }
        files[filename] = encrypted;
        return true;
    }

    bool FakeFS::updateFile(const std::string& filename, const std::string& content) {
        if (files.find(filename) == files.end()) {
            return false; // File does not exist
        }
        auto encrypted = encrypt(content);
        if (useCompression) {
            encrypted = compressData(encrypted);
        }
        files[filename] = encrypted;
        return true;
    }

    std::string FakeFS::readFile(const std::string& filename) {
        if (files.find(filename) == files.end()) {
            return ""; // File not found
        }
        auto data = files[filename];
        if (useCompression) {
            uLongf originalSize = decryptSize(data);
            data = decompressData(data, originalSize);
        }
        return decrypt(data);
    }

    bool FakeFS::deleteFile(const std::string& filename) {
        return files.erase(filename) > 0;
    }

    void FakeFS::listFiles() const {
        if (files.empty()) {
            VX_INFO("No files available.");
            return;
        }

        VX_INFO("Files:");
        for (const auto& file : files) {
            VX_INFO("- " + file.first);
        }
    }

    bool FakeFS::createRIFFFromDirectory(const std::string& folderPath, const std::string& riffFilePath, bool compress)
    {
        try {
            std::ofstream riffFile(riffFilePath, std::ios::binary);
            if (!riffFile.is_open()) {
                return false;
            }

            writeRIFFHeader(riffFile);

            for (const auto& entry : std::filesystem::recursive_directory_iterator(folderPath)) {
                if (entry.is_regular_file()) {
                    std::ifstream file(entry.path(), std::ios::binary);
                    std::vector<uint8_t> content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());

                    if (compress) {
                        // Apply compression here if needed
                        content = encrypt(std::string(content.begin(), content.end()));
                    }

                    std::string relativePath = std::filesystem::relative(entry.path(), folderPath).string();
                    writeRIFFChunk(riffFile, relativePath, content);
                }
            }

            writeRIFFFooter(riffFile);
            riffFile.close();

            VX_INFO("RIFF file created successfully.");
            return true;
        }
        catch (const std::exception& e) {
            VX_ERROR(std::string("Error creating RIFF file: ") + e.what());
            return false;
        }
    }

    bool FakeFS::writeRIFFFile(const std::string& riffFilePath) {
        try {
            std::ofstream riffFile(riffFilePath, std::ios::binary);
            if (!riffFile.is_open()) {
                return false;
            }

            writeRIFFHeader(riffFile);

            for (const auto& file : files) {
                writeRIFFChunk(riffFile, file.first, file.second);
            }

            writeRIFFFooter(riffFile);
            riffFile.close();

            VX_INFO("RIFF file written successfully.");
            return true;
        }
        catch (const std::exception& e) {
            VX_ERROR(std::string("Error writing RIFF file: ") + e.what());
            return false;
        }
    }

    bool FakeFS::readRIFFFile(const std::string& riffFilePath) {
        try {
            std::ifstream riffFile(riffFilePath, std::ios::binary);
            if (!riffFile.is_open()) {
                return false;
            }

            files.clear(); // Clear current files before loading new ones

            char header[4];
            riffFile.read(header, 4); // Read the "RIFF" header

            uint32_t fileSize;
            riffFile.read(reinterpret_cast<char*>(&fileSize), 4); // Read the file size

            riffFile.read(header, 4); // Read the "FAKE" format

            bool readUseCompression;
            riffFile.read(reinterpret_cast<char*>(&readUseCompression), 1); // Read useCompression flag

            while (riffFile) {
                char chunkName[4];
                riffFile.read(chunkName, 4);
                if (riffFile.gcount() < 4) break;

                uint32_t chunkSize;
                riffFile.read(reinterpret_cast<char*>(&chunkSize), 4);

                std::vector<uint8_t> data(chunkSize);
                riffFile.read(reinterpret_cast<char*>(data.data()), chunkSize);

                if (useChecksum) {
                    uint32_t checksum;
                    riffFile.read(reinterpret_cast<char*>(&checksum), 4);
                    uint32_t calculatedChecksum = customChecksum(data);
                    if (checksum != calculatedChecksum) {
                        VX_ERROR("Checksum mismatch for chunk: " + std::string(chunkName, 4));
                        return false;
                    }
                }

                if (readUseCompression) {
                    uLongf originalSize = decryptSize(data);
                    data = decompressData(data, originalSize);
                }

                files[std::string(chunkName, 4)] = data;
            }
            useCompression = readUseCompression;
            riffFile.close();
            VX_INFO("RIFF file read successfully.");
            return true;
        }
        catch (const std::exception& e) {
            VX_ERROR(std::string("Error reading RIFF file: ") + e.what());
            return false;
        }
    }
}