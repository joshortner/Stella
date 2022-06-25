#pragma once

// Debug Config //
#define STELLA_CONFIG_DEBUG

// Window Config //
#define STELLA_CONFIG_WINDOW_INIT_WIDTH (800*1.15)
#define STELLA_CONFIG_WINDOW_INIT_HEIGHT (600*1.15)

// Render Config //
#define STELLA_CONFIG_MAX_SHADERS 10
#define STELLA_CONFIG_RES_X 800
#define STELLA_CONFIG_RES_Y 600

#ifdef STELLA_SRC_DIR_SET
#define STELLA_ASSET_DIR STELLA_SRC_DIR    "/src/Stella/EngineAssets"
#define STELLA_SHADER_DIR STELLA_ASSET_DIR "/Shaders"
#else
#define STELLA_ASSET_DIR ""
#define STELLA_SHADER_DIR ""
#endif

#define STELLA_FULL_SCREEN_RENDER_SHADER_NAME "FullScreenQuad"
#define STELLA_BASIC_QUAD_SHADER_NAME         "BasicQuad"
#define STELLA_QUAD_RENDERER_SHADER_NAME      "QuadRenderer"

// Resource Management //
#define STELLA_TEXTURE_MANAGER_MAX_TEXTURES 10
#define STELLA_SHADER_MANAGER_MAX_SHADERS 10

// Built in shader names //
extern const char * g_stella_shader_names[];

// Shader Uniform Names //
#define STELLA_UNIFORM_NAME_PROJECTION_MATRIX "u_Projection"
#define STELLA_UNIFORM_NAME_VIEW_MATRIX       "u_View"
#define STELLA_UNIFORM_NAME_MODEL_MATRIX      "u_ModelMatrix"

// Mesh Renderer //
#define STELLA_CONFIG_MESH_RENDERER_BATCH_SIZE 10