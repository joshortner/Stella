#pragma once

#include "Stella/Core/Memory/PoolAllocator/PoolAllocator.h"

#include <string>
#include <unordered_map>

namespace Stella
{

template<typename T, int Max>
class ResourceManager
{

public:

	ResourceManager() = default;
	~ResourceManager();

	void destroy(T * p_asset) { };
	T * get(std::string name);

protected:

	T * create(std::string name);

private:

	std::unordered_map<std::string, T *> m_assetMap;
	PoolAllocator<T, Max> m_poolAllocator;

};

} // namespace Stella
