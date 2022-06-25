#pragma once

#include <unordered_map>
#include <string>

namespace Stella
{

class Sprite;

class SpriteManager
{

public:

	~SpriteManager();

    Sprite * create_sprite(std::string name, std::string file_path);
    Sprite * get_sprite(std::string name);

private:

	std::unordered_map<std::string, Sprite *> m_sprites;
};

    
} // namespace Stella
