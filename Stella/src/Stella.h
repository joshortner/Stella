#pragma once

#include "StellaDefines.h"

// Core //
#include "Stella/Core/Strings/Strings.h"
#include "Stella/Core/Memory/PoolAllocator/PoolAllocator.h"
#include "Stella/Core/Logger/Logger.h"
#include "Stella/Core/Assert/Assert.h"

// Renderer //
#include "Stella/Renderer/Texture/Texture.h"

// Resources //
#include "Stella/Resources/ResourceManager/ResourceManager.h"
#include "Stella/Resources/TextureManager/TextureManager.h"
#include "Stella/Resources/ShaderManager/ShaderManager.h"

// Debug //
#include "Stella/Debug/Print/Print.h"

/////

// Platform //
#include "Stella/Platform/Window/Window.h"
#include "Stella/Platform/Path/Path.h"

// Systems //
#include "Stella/Systems/Scene/Scene.h"
#include "Stella/Systems/Component/Component.h"
#include "Stella/Systems/NativeScriptBase/NativeScriptBase.h"
#include "Stella/Systems/ModelManager/ModelManager.h"
#include "Stella/Systems/DeltaTime/DeltaTime.h"

// Math //
#include "Stella/Math/Math2D.h"
#include "Stella/Math/Ray2D/Ray2D.h"

// Renderer //
#include "Stella/Renderer/Texture/Texture.h"
#include "Stella/Renderer/Shader/Shader.h"
#include "Stella/Renderer/QuadRenderer/QuadRenderer.h"
#include "Stella/Renderer/MeshRenderer/MeshRenderer.h"
#include "Stella/Renderer/MeshRenderer/Model.h"
#include "Stella/Renderer/SpriteRenderer/SpriteRenderer.h"
#include "Stella/Renderer/Framebuffer/Framebuffer.h"
#include "Stella/Renderer/Camera/Camera.h"
#include "Stella/Renderer/Sprite/Sprite.h"

// The Engine! //
#include "Stella/EngineState.h"