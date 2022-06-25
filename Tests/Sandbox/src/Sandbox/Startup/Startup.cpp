#include "Startup.h"
#include "SandboxConfig.h"
#include "Sandbox/Utils/Utils.h"
#include "Stella/Core/Logger/Logger.h"
#include "Stella/EngineState.h"
#include "Stella/Resources/ShaderManager/ShaderManager.h"
#include "Stella/Systems/ModelManager/ModelManager.h"
#include "Stella/Systems/SpriteManager/SpriteManager.h"
#include "Stella/Systems/Scene/Scene.h"
#include "Stella/Systems/Component/Component.h"
#include "Stella/Renderer/MeshRenderer/Model.h"
#include "Stella/Renderer/Camera/Camera.h"

#include <stdio.h>

#include <string>

using namespace Stella;

namespace Sandbox
{

static void initialize_shaders();
static void initialize_models();
static void initialize_sprites();
    
void initialize()
{
    STELLA_ENGINE_LOG("Initializing Sandbox.....");
    STELLA_ENGINE_LOG("Asset Directories %s", get_src_dir().c_str());
	STELLA_ENGINE_LOG(" - Shader Dir : %s", get_shader_dir().c_str());
	STELLA_ENGINE_LOG(" - Model Dir  : %s", get_model_dir().c_str());
	STELLA_ENGINE_LOG(" - Texture Dir: %s", get_texture_dir().c_str());

    // Load Assets //
    initialize_shaders();
    initialize_models();
    initialize_sprites();

    // Initialize Main Camera //
    // TODO: Delete camera
	float half_view = 25.0f;
	Camera * p_camera = new OrthographicCamera(
		-half_view, half_view, -half_view, half_view, 0.001f, 10000.0f, STELLA_CONFIG_WINDOW_INIT_WIDTH / STELLA_CONFIG_WINDOW_INIT_HEIGHT,
		glm::vec3(15.0f, -15.0f, 2.0f), glm::vec3(0.0f, 0.0f, -1.0f)
	);

	Entity camera_entity = EngineState::get().m_pScene->create_entity("Main Camera");
	camera_entity.add_component<CameraComponent>(p_camera);
}

void initialize_shaders()
{
    STELLA_ENGINE_LOG("Loading Sandbox Shaders...");
    for (int i = 0; i < ShaderType::MaxShaders; i++) {
        std::string vertex_path;
	    std::string fragment_path;
	    get_shader_paths(g_sandbox_shader_infos[i].m_name, vertex_path, fragment_path);
        EngineState::get().m_pShaderManager->create(g_sandbox_shader_infos[i].m_name, vertex_path, fragment_path);
        STELLA_ENGINE_LOG("Loading Shader (%s): \n\t- %s\n\t- %s", g_sandbox_shader_infos[i].m_name.c_str(), vertex_path.c_str(), fragment_path.c_str());
    }
}

void initialize_models()
{
    STELLA_ENGINE_LOG("Loading Sandbox Models...");
    for (int i = 0; i < ModelType::MaxModels; i++) {
        STELLA_ENGINE_LOG("Loading Model %s", g_sandbox_model_infos[i].m_name.c_str());
        EngineState::get().m_pModelManager->create_model(
            g_sandbox_model_infos[i].m_name, 
            get_model_path(g_sandbox_model_infos[i].m_name),
            Model::PrimitiveType::TRIANGLES
        );
    }
}

void initialize_sprites()
{
    STELLA_ENGINE_LOG("Loading Sandbox Sprites...");
    for (int i = 0; i < SpriteType::MaxSprites; i++) {
        STELLA_ENGINE_LOG("Loading Sprite %s", g_sandbox_sprite_infos[i].m_name.c_str());

        EngineState::get().m_pSpriteManager->create_sprite(
            g_sandbox_sprite_infos[i].m_name,
            get_texture_path(g_sandbox_sprite_infos[i].m_name, g_sandbox_sprite_infos[i].m_ext)
        );
    }

}

// Globals //

ShaderAssetInfo g_sandbox_shader_infos[]
{
    { SANDBOX_SHADER_TEST_SHADER } 
};

SpriteAssetInfo g_sandbox_sprite_infos[]
{
    { SANDBOX_TEXTURE_NAME_CONTAINER, ".jpg" },
    { SANDBOX_TEXTURE_NAME_TEST_PX_ART, ".png" },
    { SANDBOX_TEXTURE_NAME_RED_GEM, ".png" }
};

ModelAssetInfo g_sandbox_model_infos[]
{
    { SANDBOX_MODEL_NAME_WELL },
    { SANDBOX_MODEL_NAME_CUBE },
    { SANDBOX_MODEL_NAME_TUG_SHIP }
};


} // namespace Sandbox
