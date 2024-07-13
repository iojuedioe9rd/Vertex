#include "vxpch.h"
#include "FakeFS.h"
namespace Vertex
{

    VERTEX_API std::string readFile(const FakeFS& fakefs, const std::string& path) {
        try {
            File file = fakefs.getFile(path);
            return file.content;
        }
        catch (const std::runtime_error& e) {
            return e.what();
        }
    }
}