#pragma once
#include <Vertex/Core/Base.h>
#include <cstdio>
#include <string.h>

namespace Vertex
{
	/*! \brief Allocates from global memory (NOTE: does not currently align memory) */
	struct VERTEX_API GlobalAllocator
	{
		void* Allocate(size_t sizeBytes);
		

		void DeAllocate(void* ptr, size_t /*osize*/);
		

		void* ReAllocate(void* ptr, size_t osize, size_t nsize);
		

		static void* l_alloc(void* ud, void* ptr, size_t osize, size_t nsize);
	};

	/*! \brief Allocates from a fixed pool.
	*	Aligns all memory to 8 bytes
	*	Has a min allocation of 64 bytes
	*	Puts all free'd blocks on a free list.
	*	Falls back to GlobalAllocator when out of memory. */
	struct VERTEX_API ArenaAllocator
	{
		void* m_begin;
		void* m_end;
		char* m_curr;

		static constexpr int ALIGNMENT = 8;
		static constexpr int MIN_BLOCK_SIZE = ALIGNMENT * 8;

		struct VERTEX_API FreeList
		{
			FreeList* m_next;
		};

		FreeList* m_freeListHead;
		GlobalAllocator m_globalAllocator;

		ArenaAllocator(void* begin, void* end) :
			m_begin(begin),
			m_end(end)
		{
			Reset();
		}

		void Reset();
		

		size_t SizeToAllocate(size_t size);
		

		void* Allocate(size_t sizeBytes);

		void DeAllocate(void* ptr, size_t osize);
		

		void* ReAllocate(void* ptr, size_t osize, size_t nsize);
		

		static void* l_alloc(void* ud, void* ptr, size_t osize, size_t nsize);
	};
}

