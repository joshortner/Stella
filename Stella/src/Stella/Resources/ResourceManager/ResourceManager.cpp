#include "ResourceManager.h"
#include "StellaConfig.h"
#include "Stella/Renderer/Texture/Texture.h"
#include "Stella/Renderer/Shader/Shader.h"
#include "Stella/Core/Logger/Logger.h"

namespace Stella
{

template<typename T, int Max>
ResourceManager<T, Max>::~ResourceManager()
{
    for (auto & [key, p_asset] : m_assetMap) {
        destroy(p_asset);
        m_poolAllocator.free(p_asset);
    }
}

template<typename T, int Max>
T * ResourceManager<T, Max>::create(std::string name)
{
    if (m_assetMap.find(name) != m_assetMap.end()) {
        STELLA_LOG(Logger::ERROR, "Asset name already in use - %s", name.c_str());
        return nullptr;
    }
    m_assetMap[name] = m_poolAllocator.alloc();
    return m_assetMap[name];
}

template<typename T, int Max>
T * ResourceManager<T, Max>::get(std::string name)
{
    if (m_assetMap.find(name) == m_assetMap.end()) {
        STELLA_LOG(Logger::ERROR, "Asset doesn't exist - %s", name.c_str());
        return nullptr;
    }
    return m_assetMap[name];
}

template class ResourceManager<Texture2D, STELLA_TEXTURE_MANAGER_MAX_TEXTURES>;
template class ResourceManager<Shader, STELLA_SHADER_MANAGER_MAX_SHADERS>;

} // namespace Stella
