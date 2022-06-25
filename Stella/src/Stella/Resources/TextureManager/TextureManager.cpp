#include "TextureManager.h"
#include "Stella/Core/Assert/Assert.h"

namespace Stella
{

Texture2D * Texture2DManager::create(std::string name, const Texture2DInfo & info)
{
    Texture2D * p_tex = ResourceManager::create(name);
    STELLA_ASSERT((p_tex != nullptr), "Error creating texture");
    STELLA_ASSERT((Texture2D::create(p_tex, info)), "Error initializing texture");
    return p_tex;
}

void Texture2DManager::destroy(Texture2D * texture)
{
    Texture2D::destroy(texture);
}
    
} // namespace Stella
