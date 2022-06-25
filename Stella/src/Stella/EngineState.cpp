#include "EngineState.h"
#include "Stella/Platform/Platform.h"
#include "Stella/Core/Assert/Assert.h"
#include "Stella/Core/Logger/Logger.h"
#include "Stella/Math/Ray2D/Ray2D.h"
#include "Stella/Platform/Window/Window.h"
#include "Stella/Systems/Scene/Scene.h"
//#include "Stella/Systems/ShaderManager/ShaderManager.h"
#include "Stella/Systems/ModelManager/ModelManager.h"
#include "Stella/Systems/SpriteManager/SpriteManager.h"
#include "Stella/Systems/Component/Component.h"
#include "Stella/Renderer/MeshRenderer/MeshRenderer.h"
#include "Stella/Renderer/SpriteRenderer/SpriteRenderer.h"
#include "Stella/Renderer/Framebuffer/Framebuffer.h"
#include "Stella/Renderer/QuadRenderer/QuadRenderer.h"
#include "Stella/Resources/TextureManager/TextureManager.h"
#include "Stella/Resources/ShaderManager/ShaderManager.h"

#include <stdio.h>

#define STELLA_ENGINE_STATE_ASSERT_NOT_INIT() STELLA_ASSERT((s_pState == nullptr), "Stella Engine State is already initialized!")
#define STELLA_ENGINE_STATE_ASSERT_INIT() STELLA_ASSERT((s_pState != nullptr), "Stella Engine State is not initialized!")

const char * g_stella_shader_names[] = {
    STELLA_FULL_SCREEN_RENDER_SHADER_NAME,
    STELLA_BASIC_QUAD_SHADER_NAME,
    STELLA_QUAD_RENDERER_SHADER_NAME
};

namespace Stella
{
    
EngineState * EngineState::s_pState = nullptr;

EngineState::EngineState()
{

#ifdef STELLA_CONFIG_DEBUG
    Logger::initialize();
#endif

    m_pWindow           = new Window(STELLA_CONFIG_WINDOW_INIT_WIDTH, STELLA_CONFIG_WINDOW_INIT_HEIGHT);
    m_pFrameBuffer      = new Framebuffer(STELLA_CONFIG_RES_X, STELLA_CONFIG_RES_Y);
    m_pScene            = new Scene();
    m_pMeshRenderer     = new MeshRenderer();
    m_pSpriteRenderer   = new SpriteRenderer();
    m_pShaderManager    = new ShaderManager();
    m_pModelManager     = new ModelManager();
    m_pSpriteManager    = new SpriteManager();
    m_pTexture2DManager = new Texture2DManager();

#ifdef STELLA_CONFIG_DEBUG
    m_pDebugQuadRenderer = new QuadRenderer(); // After OpenGL init
#endif

    // Full Screen Quad Model //
    m_pFullScreenQuad = m_pModelManager->create_model("FullScreenQuad", Model::PrimitiveType::TRIANGLES);
    m_pFullScreenQuad->vertices = {
        { -1.0f,  1.0f, 0.0f },
        { -1.0f, -1.0f, 0.0f },
        {  1.0f, -1.0f, 0.0f },
        { -1.0f,  1.0f, 0.0f },
        {  1.0f, -1.0f, 0.0f },
        {  1.0f,  1.0f, 0.0f }
    };
    m_pFullScreenQuad->uvs = {
        { 0.0f, 1.0f },
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f }
    };
    m_pFullScreenQuad->build();
}

EngineState::~EngineState()
{
    delete m_pWindow;
    delete m_pScene;
    delete m_pMeshRenderer;
    delete m_pSpriteRenderer;
    delete m_pShaderManager;
    delete m_pFrameBuffer;
    delete m_pSpriteManager;
    delete m_pModelManager;
    delete m_pTexture2DManager;

#ifdef STELLA_CONFIG_DEBUG
    delete m_pDebugQuadRenderer;
    Logger::destroy(); // Do last so other systems can log deallocation
#endif
}

void EngineState::initialize()
{
    STELLA_ENGINE_STATE_ASSERT_NOT_INIT();
    s_pState = new EngineState();

    // Initialize engine assets //

    // Shaders
    for (int i = 0; i < sizeof(g_stella_shader_names) / sizeof(const char *); i++) {
        s_pState->m_pShaderManager->create(
            g_stella_shader_names[i], 
            EngineState::get_stella_shader_path(g_stella_shader_names[i], VERT),
            EngineState::get_stella_shader_path(g_stella_shader_names[i], FRAG)
        );
    }

    // Initialize OpenGL things //
    glEnable(GL_DEPTH_TEST);
}

void EngineState::destroy()
{
    STELLA_ENGINE_STATE_ASSERT_INIT();
    delete s_pState;
}

void EngineState::on_update()
{
    STELLA_ENGINE_STATE_ASSERT_INIT();
    s_pState->m_dt.tick();
    s_pState->m_pScene->on_update(s_pState->m_dt);
    //printf("%f\n", 1.0f / (float)s_pState->m_dt);
}

void EngineState::on_render()
{
    STELLA_ENGINE_STATE_ASSERT_INIT();

    // Render Scene //
    s_pState->m_pFrameBuffer->bind();
    Framebuffer::clear(glm::vec3(0.3f, 0.3f, 0.3f));	
    s_pState->m_pScene->on_render();

    Debug::flush();	// Debug draws
    s_pState->m_pFrameBuffer->unbind();

    // Render full screen quad to default framebuffer //
    glm::vec2 fb_dims = s_pState->m_pWindow->get_framebuffer_dims();
    glViewport(0, 0, fb_dims.x, fb_dims.y);

    Framebuffer::clear(glm::vec3(1.0f, 0.0f, 1.0f));

    Shader * p_fs_shader = s_pState->m_pShaderManager->get(STELLA_FULL_SCREEN_RENDER_SHADER_NAME);
    p_fs_shader->use();
    p_fs_shader->set_int("screen_texture", 0);
    s_pState->m_pFullScreenQuad->bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, s_pState->m_pFrameBuffer->m_colorAttachment);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void EngineState::on_finish_frame()
{
    STELLA_ENGINE_STATE_ASSERT_INIT();
    s_pState->m_pWindow->swap_buffers();
    s_pState->m_pWindow->poll_events();
}

EngineState & EngineState::get()
{
    STELLA_ENGINE_STATE_ASSERT_INIT();
    return *s_pState;
}

std::string EngineState::get_stella_src_dir()
{
#ifdef STELLA_SRC_DIR_SET
    return std::string(STELLA_SRC_DIR);
#else
    STELLA_ASSERT((0), "Project directory not set!");
#endif
}

std::string EngineState::get_stella_shader_path(const std::string & name, ShaderExt ext)
{
#ifdef STELLA_SRC_DIR_SET
    switch (ext)
    {
        case VERT: return std::string(STELLA_SHADER_DIR) + "/" + name + "/" + name + ".vert";
        case FRAG: return std::string(STELLA_SHADER_DIR) + "/" + name + "/" + name + ".frag";
    }
#else
    STELLA_ASSERT((0), "Project directory not set!");
#endif
}

namespace Debug
{

void draw_quad(const glm::mat4 & transform, const glm::vec4 & color)
{
    glm::vec4 top_right    = glm::vec4( 0.5f,  0.5f, 0.0f, 1.0f); // top right
    glm::vec4 bottom_right = glm::vec4( 0.5f, -0.5f, 0.0f, 1.0f); // bottom right
    glm::vec4 bottom_left  = glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f); // bottom left
    glm::vec4 top_left     = glm::vec4(-0.5f,  0.5f, 0.0f, 1.0f); // top left 

    top_right    = transform * top_right;
    bottom_right = transform * bottom_right;
    bottom_left  = transform * bottom_left;
    top_left     = transform * top_left;

    EngineState::get().m_quadBuffer.push_back(Debug::Quad(top_right, top_left, bottom_right, bottom_left, color));

    //push_quad(top_right, bottom_right, bottom_left, top_left, color);
}

void draw_line(const glm::vec3 & p0, const glm::vec3 & p1, const glm::vec4 & color, float width)
{
    glm::vec3 left_point  = p0;
    glm::vec3 right_point = p1;
    if (p0.x > p1.x) {
        left_point  = p1;
        right_point = p0;
    }

    glm::vec3 diff = right_point - left_point;
    glm::vec3 dir = glm::normalize(glm::vec3(-diff.y, diff.x, diff.z));
    dir *= width / 2.0f;

    glm::vec3 top_left     = left_point + dir;
    glm::vec3 bottom_left  = left_point - dir;
    glm::vec3 top_right    = right_point + dir;
    glm::vec3 bottom_right = right_point - dir;

    //push_quad(top_right, bottom_right, bottom_left, top_left, color);
    EngineState::get().m_quadBuffer.push_back(Debug::Quad(top_right, top_left, bottom_right, bottom_left, color));
}

void draw_line(const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec4 & color, float width)
{
    glm::vec3 p30 = glm::vec3(p0.x, p0.y, 0.0f);
    glm::vec3 p31 = glm::vec3(p1.x, p1.y, 0.0f);
    draw_line(p30, p31, color, width);
}

void draw_ray2d(const Ray2D & ray, const glm::vec4 & color, float width)
{
    Debug::draw_line(ray.m_origin, ray.m_origin + ray.m_dir * ray.m_length, color, width);
}

void flush()
{
    Camera * p_main_cam = nullptr;

    // Get main scene camera //
    {
        auto view = EngineState::get().m_pScene->view<CameraComponent>();

        for (auto [entity, cam_comp] : view.each()) {
            p_main_cam = cam_comp.m_pCamera;
            break;
        }
    }

    if (p_main_cam == nullptr) {
        STELLA_LOG(Logger::WARN, "Scene has no camera!");
    }

    EngineState::get().m_pDebugQuadRenderer->start(p_main_cam);

    for (Debug::Quad & quad : EngineState::get().m_quadBuffer) {
        EngineState::get().m_pDebugQuadRenderer->push_quad(quad.m_tr, quad.m_br, quad.m_bl, quad.m_tl, quad.m_color);
    }

    EngineState::get().m_pDebugQuadRenderer->end();

    EngineState::get().m_quadBuffer.clear();
}
    
} // namespace Debug

} // namespace Stella
