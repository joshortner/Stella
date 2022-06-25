#include "Shader.h"
#include "Stella/Core/Logger/Logger.h"
#include "Stella/Core/Assert/Assert.h"

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace Stella
{

// SOURCE: https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader.h

Shader::Shader(int id) :
    m_id(id)
{

}

/*
Shader Shader::create(const char * vertex_path, const char * fragment_path)
{
    int id;

    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertex_code;
    std::string fragment_code;
    std::ifstream v_shader_file;
    std::ifstream f_shader_file;

    // ensure ifstream objects can throw exceptions:
    v_shader_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    f_shader_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try {
        // open files
        v_shader_file.open(vertex_path);
        f_shader_file.open(fragment_path);
        std::stringstream vShaderStream, fShaderStream;
        
        // read file's buffer contents into streams
        vShaderStream << v_shader_file.rdbuf();
        fShaderStream << f_shader_file.rdbuf();		
        
        // close file handlers
        v_shader_file.close();
        f_shader_file.close();

        // convert stream into string
        vertex_code = vShaderStream.str();
        fragment_code = fShaderStream.str();			
    }
    catch (std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* v_shader_code = vertex_code.c_str();
    const char * f_shader_code = fragment_code.c_str();
    
    // 2. compile shaders
    unsigned int vertex, fragment;

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &v_shader_code, NULL);
    glCompileShader(vertex);
    check_compile_errors(vertex, "VERTEX");

    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &f_shader_code, NULL);
    glCompileShader(fragment);
    check_compile_errors(fragment, "FRAGMENT");

    // shader Program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    check_compile_errors(id, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    
    return Shader(id);
}*/

bool Shader::create(Shader * shader, const char * vertex_path, const char * fragment_path)
{
    int id;

    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertex_code;
    std::string fragment_code;
    std::ifstream v_shader_file;
    std::ifstream f_shader_file;

    // ensure ifstream objects can throw exceptions:
    v_shader_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    f_shader_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try {
        // open files
        v_shader_file.open(vertex_path);
        f_shader_file.open(fragment_path);
        std::stringstream vShaderStream, fShaderStream;
        
        // read file's buffer contents into streams
        vShaderStream << v_shader_file.rdbuf();
        fShaderStream << f_shader_file.rdbuf();		
        
        // close file handlers
        v_shader_file.close();
        f_shader_file.close();

        // convert stream into string
        vertex_code = vShaderStream.str();
        fragment_code = fShaderStream.str();			
    }
    catch (std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        return false;
    }
    const char* v_shader_code = vertex_code.c_str();
    const char * f_shader_code = fragment_code.c_str();
    
    // 2. compile shaders
    unsigned int vertex, fragment;

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &v_shader_code, NULL);
    glCompileShader(vertex);
    check_compile_errors(vertex, "VERTEX");

    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &f_shader_code, NULL);
    glCompileShader(fragment);
    check_compile_errors(fragment, "FRAGMENT");

    // shader Program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    check_compile_errors(id, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    shader->m_id = id;
    STELLA_LOG(Logger::DEBUG, "\n\tShader created (%d)\n\t %s\n\t %s\n", id, vertex_path, fragment_path);
    return true;
}

void Shader::destroy(Shader shader)
{
    STELLA_ASSERT((shader.m_id != STELLA_INVALID_SHADER_ID), "Invalid Shader!");
    glDeleteProgram(shader.m_id);
    shader.m_id = STELLA_INVALID_SHADER_ID;
}

void Shader::use() 
{
    STELLA_ASSERT((m_id != STELLA_INVALID_SHADER_ID), "Invalid Shader!");
    glUseProgram(m_id);
}

int Shader::get_id() const
{
    return m_id;
}

void Shader::set_bool(const std::string & name, bool value) const 
{
    STELLA_ASSERT((m_id != STELLA_INVALID_SHADER_ID), "Invalid Shader!");
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value); 
}

void Shader::set_int(const std::string & name, int value) const 
{
    STELLA_ASSERT((m_id != STELLA_INVALID_SHADER_ID), "Invalid Shader!");
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::set_float(const std::string & name, float value) const 
{
    STELLA_ASSERT((m_id != STELLA_INVALID_SHADER_ID), "Invalid Shader!");
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::set_float3(const std::string & name, const glm::vec3 & vec) const 
{
    STELLA_ASSERT((m_id != STELLA_INVALID_SHADER_ID), "Invalid Shader!");
    glUniform3f(glGetUniformLocation(m_id, name.c_str()), vec.x, vec.y, vec.z);
}

void Shader::set_mat4(const std::string & name, const glm::mat4 & mat) const
{
    STELLA_ASSERT((m_id != STELLA_INVALID_SHADER_ID), "Invalid Shader!");
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::set_int_array(const std::string & name, uint32_t count, const int * data) const
{
    glUniform1iv(glGetUniformLocation(m_id, name.c_str()), count, data);
}

void Shader::check_compile_errors(int shader, std::string type)
{
    GLint success;
    GLchar infoLog[1024];
    if(type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            throw std::runtime_error("Shader Compilation Error");
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            throw std::runtime_error("Shader Compilation Error");
        }
    }
}

} // namespace Stella
