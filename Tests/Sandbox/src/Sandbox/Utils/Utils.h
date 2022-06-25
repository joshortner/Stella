#pragma once

#include <string>

namespace Sandbox
{

std::string get_src_dir();
std::string get_shader_dir();
std::string get_model_dir();
std::string get_texture_dir();

std::string get_texture_path(std::string file_name, std::string file_ext);
std::string get_model_path(std::string model_name);
void get_shader_paths(std::string name, std::string & vert_out, std::string & frag_out);
    
} // namespace Sandbox
