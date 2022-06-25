#pragma once

#include <glm/glm.hpp>

#include <stdint.h>

#include <string>

#define STELLA_INVALID_SHADER_ID (-1)

namespace Stella
{

class Shader
{
public:

    Shader() = default;
    Shader(const Shader & obj) = default;
    Shader(int id);
    ~Shader() = default;

    //static Shader create(const char * vertex_path, const char * fragment_path);
    static bool create(Shader * shader, const char * vertex_path, const char * fragment_path);
    static void destroy(Shader shader);

    void use();

    int get_id() const;

    void set_bool(const std::string & name, bool value) const;
    void set_int(const std::string & name, int value) const;
    void set_float(const std::string & name, float value) const;
    void set_float3(const std::string & name, const glm::vec3 & vec) const;
    void set_mat4(const std::string & name, const glm::mat4 & mat) const;
    void set_int_array(const std::string & name, uint32_t count, const int * data) const;

private:

    static void check_compile_errors(int shader, std::string type);

    int m_id;
};
    
} // namespace Stella
