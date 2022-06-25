#pragma once

#include "Stella/Core/Assert/Assert.h"
#include "Stella/Core/Logger/Logger.h"

#include <string.h>

#include <string>
#include <list>

namespace Stella
{

template<typename T, int M>
class PoolAllocator
{
public:

	PoolAllocator();

	T * alloc();
	void free(T * ptr);

	std::string to_string();

private:

	T m_Buffer[M];
	std::list<uint32_t> m_FreeList;

};

template<typename T, int M>
PoolAllocator<T, M>::PoolAllocator()
{
	for (int i = 0; i < M; i++) {
		m_FreeList.push_back(i);
	}
}

template<typename T, int M>
T * PoolAllocator<T, M>::alloc()
{
	if (m_FreeList.size() == 0) {
		STELLA_LOG(Logger::FATAL, "Chunk Memory Manager Out of Memory!");
		return nullptr;
	}
	uint32_t idx = m_FreeList.front();
	m_FreeList.pop_front();

	STELLA_LOG
	(
		Logger::DEBUG,
		"\n\tPool Allocator %s\n\t  Allocating chunk (%d:%p)\n\t   %d of %d chunks allocated\n",
		to_string().c_str(),
		idx, &m_Buffer[idx],
		M - m_FreeList.size(), M
	);

	return &m_Buffer[idx];
}

template<typename T, int M>
void PoolAllocator<T, M>::free(T * ptr)
{
	uint32_t idx = ptr - &m_Buffer[0];
	//STELLA_ASSERT((idx < M && idx >= 0), "Freed invalid pointer!");
	if (idx < M && idx >= 0) {
		m_FreeList.push_back(idx);
		STELLA_LOG
		(
			Logger::DEBUG,
			"\n\tPool Allocator %s\n\t  Freeing chunk (%d:%p)\n\t   %d of %d chunks allocated\n",
			to_string().c_str(),
			idx, &m_Buffer[idx],
			M - m_FreeList.size(), M
		);
	}
	else {
		Logger::log(Logger::WARN, "Freed invalid pointer!");
	}
}

template<typename T, int M>
std::string PoolAllocator<T, M>::to_string()
{
	const static uint32_t MAX_BUF_LEN = 256;
	char buf[MAX_BUF_LEN] = { 0 };
	snprintf(buf, MAX_BUF_LEN, "{ type: %s, chunk size: %lu, max chunks: %d, total size: %lu, addr space: %p - %p }", 
								typeid(T).name(), sizeof(T), M, sizeof(m_Buffer), &m_Buffer[0], &m_Buffer[M-1] );
	return buf;
}
    
} // namespace Stella
