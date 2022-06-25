#include "Utils.h"
#include "Stella/Core/Assert/Assert.h"

namespace Sandbox
{

std::string get_src_dir()
{
#ifdef PROJECT_SRC_DIR_SET
	return std::string(PROJECT_SRC_DIR);
#else
	STELLA_ASSERT((0), "Project directory not set!");
#endif
}

std::string get_shader_dir()
{
	return get_src_dir() + std::string("/Assets/Shaders");
}

std::string get_model_dir()
{
	return get_src_dir() + std::string("/Assets/Models");
}

std::string get_texture_dir()
{
	return get_src_dir() + std::string("/Assets/Textures");
}

std::string get_texture_path(std::string file_name, std::string file_ext)
{
	return get_texture_dir() + "/" + file_name + file_ext;
}

std::string get_model_path(std::string model_name)
{
	return get_model_dir() + "/" + model_name + ".obj";
}

void get_shader_paths(std::string name, std::string & vert_out, std::string & frag_out)
{
	std::string shader_dir = get_shader_dir() + "/" + name;
	vert_out = shader_dir + "/" + name + ".vert";
	frag_out = shader_dir + "/" + name + ".frag";
}
    
} // namespace Sandbox
