#pragma once
#include <Vertex/Core/Base.h>
#include <string>

typedef std::string UUID;

namespace Vertex
{
	class VERTEX_API IntUUID
	{
	public:
		IntUUID();
		IntUUID(uint64_t uuid);
		IntUUID(const IntUUID&) = default;

		operator uint64_t() const { return m_UUID; }
	private:
		uint64_t m_UUID;
	};
}

namespace std {
	template <typename T> struct VERTEX_API hash;

	template<>
	struct VERTEX_API hash<Vertex::IntUUID>
	{
		std::size_t operator()(const Vertex::IntUUID& uuid) const
		{
			return (uint64_t)uuid;
		}
	};

}

UUID VERTEX_API get_uuid();