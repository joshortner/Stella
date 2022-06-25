#pragma once

#include "StellaConfig.h"
#include "Stella/Renderer/Texture/Texture.h"
#include "Stella/Resources/ResourceManager/ResourceManager.h"

#include <string>

namespace Stella
{

class Texture2DManager : public ResourceManager<Texture2D, STELLA_TEXTURE_MANAGER_MAX_TEXTURES>
{
public:

	Texture2DManager() = default;
	~Texture2DManager() = default;

	Texture2D * create(std::string name, const Texture2DInfo & info);
	void destroy(Texture2D * texture);

};
    
} // namespace Stella
