#pragma once

#include "StellaConfig.h"
#include "Stella/Systems/DeltaTime/DeltaTime.h"

#include <glm/glm.hpp>

#include <string>
#include <vector>

#define STELLA_ENGINE_GET_SCENE() (*EngineState::get().m_pScene)
//#define STELLA_ENGINE_LOG(fmt, ...) { printf("[STELLA LOG] "); EngineState::get().m_pLogger->log(fmt, ##__VA_ARGS__); }

namespace Stella
{

// Forward Declares //
class Ray2D;
class Logger;
class Window;
class Scene;
class QuadRenderer;
class MeshRenderer;
class SpriteRenderer;
class Framebuffer;
class ModelManager;
class SpriteManager;
class Texture2DManager;
class ShaderManager;
class Model;

namespace Debug
{

void draw_quad(const glm::mat4 & transform, const glm::vec4 & color);
void draw_line(const glm::vec3 & p0, const glm::vec3 & p1, const glm::vec4 & color, float width);
void draw_line(const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec4 & color, float width);
void draw_ray2d(const Ray2D & ray, const glm::vec4 & color, float width);
void flush();

struct Quad
{
	glm::vec3 m_tr;
	glm::vec3 m_tl;
	glm::vec3 m_br;
	glm::vec3 m_bl;
	glm::vec4 m_color;

	Quad() = default;
	Quad(glm::vec3 tr, glm::vec3 tl, glm::vec3 br, glm::vec3 bl, glm::vec4 color) :
		m_tr(tr), m_tl(tl), m_br(br), m_bl(bl), m_color(color) { }
};

} // namespace Debug

struct EngineState
{
    DeltaTime m_dt;

#ifdef STELLA_CONFIG_DEBUG
    Logger * m_pLogger;
    QuadRenderer * m_pDebugQuadRenderer;
	std::vector<Debug::Quad> m_quadBuffer;
#endif

    Window *         m_pWindow;
    Scene *          m_pScene;
    MeshRenderer *   m_pMeshRenderer;
    SpriteRenderer * m_pSpriteRenderer;
    ShaderManager *  m_pShaderManager;
    Framebuffer *    m_pFrameBuffer;
    ModelManager *   m_pModelManager;
    SpriteManager *  m_pSpriteManager;
    Texture2DManager * m_pTexture2DManager;

    Model * m_pFullScreenQuad;

public:

    EngineState();
    ~EngineState();

    static void initialize();
    static void destroy();

    static void on_update();
    static void on_render();
    static void on_finish_frame();

    static EngineState & get();

private:

    std::string get_stella_src_dir();

enum ShaderExt
{
    VERT, FRAG
};
    static std::string get_stella_shader_path(const std::string & name, ShaderExt ext);

    static EngineState * s_pState;

};

} // namespace Stella
