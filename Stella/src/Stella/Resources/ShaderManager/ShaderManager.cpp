#include "ShaderManager.h"
#include "Stella/Core/Assert/Assert.h"

namespace Stella
{

Shader * ShaderManager::create(std::string name, std::string vertex_shader_path, std::string fragment_shader_path)
{
    Shader * shader = ResourceManager::create(name);
    STELLA_ASSERT((shader != nullptr), "Error creating shader");
    STELLA_ASSERT((Shader::create(shader, vertex_shader_path.c_str(), fragment_shader_path.c_str())), "Error initializing shader");
    return shader;
}

void ShaderManager::destroy(Shader * asset)
{
    Shader::destroy(*asset);
}
    
} // namespace Stella
