#pragma once

#include "StellaConfig.h"
#include "Stella/Renderer/Shader/Shader.h"
#include "Stella/Resources/ResourceManager/ResourceManager.h"

#include <string>

namespace Stella
{

class ShaderManager : public ResourceManager<Shader, STELLA_SHADER_MANAGER_MAX_SHADERS>
{
public:

	ShaderManager() = default;
	~ShaderManager() = default;

	Shader * create(std::string name, std::string vertex_shader_path, std::string fragment_shader_path);
    void destroy(Shader * asset);

};
    
} // namespace Stella
