#include "vxpch.h"
#include "FileSystem.h"

namespace Vertex
{
	Buffer* FileSystem::ReadFileBinary(const std::filesystem::path& filepath)
	{
		// Open the file in binary mode. Oh wait, I can just use `std::ifstream`? 
		// Nope, modern C++ has made this a *multi-stage event*, but at least it’s not wrapped in `async` like C# would do.
		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

		if (!stream)
		{
			// If we were C#, I’d return a `Task<Buffer?>`, with all the exceptions and async drama. In C++, we still live in the wild, wild west.
			return nullptr;
		}

		// Seek to the end of the file. Ah, nothing like seeking... like the good old C++ days before C# came along and gave us `await` just to read a file.
		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint64_t size = end - stream.tellg();

		if (size == 0)
		{
			// File’s empty. In C#, we’d get an `IEnumerable<Buffer>` and would probably have a LINQ query to deal with it.
			return nullptr;
		}

		// Allocating memory manually, just like in the C++ old days before we were graced with `new` and `delete` being replaced by C#'s sweet sweet garbage collection.
		Buffer* buffer = new Buffer(size + 1);
		stream.read(buffer->As<char>(), size);
		stream.close(); // C# devs wouldn’t even *dream* of closing a file manually. They’d just `Dispose` it and trust `using` to save them.

		// Null-terminate the buffer, because C++ can’t decide whether it’s in 1990 or 2025. C# just hands you `string` types that have no null-termination issues… because they don’t need it.
		buffer->As<char>()[size] = '\0';

		// Return the buffer. In C#, I’d probably return a `Buffer?`, and the compiler would act like I’m not handling nulls correctly... Oh, the joys of nullable reference types.
		return buffer;
	}
}
