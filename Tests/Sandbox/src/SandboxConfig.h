#pragma once

#include <string>

// Models
#define SANDBOX_MODEL_NAME_WELL "well"
#define SANDBOX_MODEL_NAME_CUBE "cube"
#define SANDBOX_MODEL_NAME_TUG_SHIP "TugShip"

// Textures
#define SANDBOX_TEXTURE_NAME_CONTAINER "container"
#define SANDBOX_TEXTURE_NAME_TEST_PX_ART "test_px_art"
#define SANDBOX_TEXTURE_NAME_RED_GEM "red_gem"

// Shaders //
#define SANDBOX_SHADER_TEST_SHADER "TestShader"

namespace Sandbox
{

// Shader Info //

enum ShaderType
{
    ShaderType_TestShader = 0,
    MaxShaders
};

struct ShaderAssetInfo
{
    std::string m_name;
};

extern ShaderAssetInfo g_sandbox_shader_infos[MaxShaders];

// Sprite Infos //

enum SpriteType
{
    SpriteType_Container,
    SpriteType_TestPxArt,
    SpriteType_RedGem,
    MaxSprites
};

struct SpriteAssetInfo
{
    std::string m_name;
    std::string m_ext;
};

extern SpriteAssetInfo g_sandbox_sprite_infos[MaxSprites];

// Model Info //

enum ModelType
{
    ModelType_Well = 0,
    ModelType_Cube,
    ModelType_TugShip,
    MaxModels
};

struct ModelAssetInfo
{
    std::string m_name;
};

extern ModelAssetInfo g_sandbox_model_infos[MaxModels];

}