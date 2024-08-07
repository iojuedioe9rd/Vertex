#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include "Vertex/Core/Base.h"

namespace Vertex
{


    // Define a structure to represent a file
    struct File {
        std::string name;
        std::string metadata;
        std::string content;
        uint16_t checksum;

        File(const std::string& name, const std::string& content)
            : name(name), content(content), metadata("") {}

        File(const std::string& name, const std::string& content, const std::string& metadata)
            : name(name), content(content), metadata(metadata) {}

        File() {}
    };

    // Define a structure to represent the fake file system
    struct FakeFS {
        std::unordered_map<std::string, File> files;

        void addFile(const std::string& path, const File& file) {
            files[path] = file;
        }

        bool fileExists(const std::string& path) const {
            return files.find(path) != files.end();
        }

        const File& getFile(const std::string& path) const {
            return files.at(path);
        }

        void addFS(FakeFS& fs)
        {
            for (auto& it : fs.files) {
                // Do stuff
                printf("1\n");
                files.insert(it);
            }
        }
    };

    VERTEX_API std::string readFile(const FakeFS& fakefs, const std::string& path);
}