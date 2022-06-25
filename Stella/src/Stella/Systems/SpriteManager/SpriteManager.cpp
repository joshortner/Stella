#include "SpriteManager.h"
#include "Stella/Core/Assert/Assert.h"
#include "Stella/Renderer/Sprite/Sprite.h"

namespace Stella
{

SpriteManager::~SpriteManager()
{
    for (auto [key, p_sprite] : m_sprites) {
        delete p_sprite;
    }
}

Sprite * SpriteManager::create_sprite(std::string name, std::string file_path)
{
    STELLA_ASSERT((m_sprites.find(name) == m_sprites.end()), "Sprite name already in use: %s", name.c_str());
    Sprite * p_sprite = new Sprite(file_path);
    m_sprites[name] = p_sprite;
    return p_sprite;
}

Sprite * SpriteManager::get_sprite(std::string name)
{
    STELLA_ASSERT((m_sprites.find(name) != m_sprites.end()), "Sprite doesn't exist %s", name.c_str());
    return m_sprites[name];
}
    
} // namespace Stella
