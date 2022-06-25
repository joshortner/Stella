cmake_minimum_required(VERSION 3.7.1)
set(CMAKE_CXX_STANDARD 17)

project(stella)

option(BOX2D_BUILD_UNIT_TESTS "Build the Box2D unit tests" OFF)
option(BOX2D_BUILD_TESTBED "Build the Box2D testbed" OFF)

#####SET VARIABLES
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(STELLA_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(GLFW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw)
set(GLAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/glad)
set(GLM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/glm)
set(ENTT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/entt/src/entt)
set(TINY_OBJ_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/tinyobjloader)
set(STB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/stb)
set(BOX2D_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/box2d)
set(OPEN_CL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/OpenCL-CLHPP)

add_subdirectory(${GLFW_DIR})
add_subdirectory(${BOX2D_DIR})
#add_subdirectory(${OPEN_CL_DIR})

find_package(OpenCL REQUIRED)
#find_package(OpenCLHeaders REQUIRED)
#find_package(OpenCLICDLoader REQUIRED)
#find_package(OpenCLHeadersCpp REQUIRED)
#target_link_libraries(app PRIVATE OpenCL::Headers OpenCL::OpenCL OpenCL::HeadersCpp)

file(
	GLOB SOURCES
	${STELLA_SRC_DIR}/Stella/EngineState.cpp

	##### Core #####
	${STELLA_SRC_DIR}/Stella/Core/Strings/*cpp
	${STELLA_SRC_DIR}/Stella/Core/Memory/PoolAllocator*cpp
	${STELLA_SRC_DIR}/Stella/Core/Logger/*cpp

	##### Resources #####
	${STELLA_SRC_DIR}/Stella/Resources/ResourceManager/*cpp
	${STELLA_SRC_DIR}/Stella/Resources/TextureManager/*cpp
	${STELLA_SRC_DIR}/Stella/Resources/ShaderManager/*cpp

	##### Debug #####
	${STELLA_SRC_DIR}/Stella/Debug/Print/*cpp

	##### Platform #####
	${STELLA_SRC_DIR}/Stella/Platform/Window/*cpp
	${STELLA_SRC_DIR}/Stella/Platform/Path/*cpp

	##### Math #####
	${STELLA_SRC_DIR}/Stella/Math/*cpp

	##### Systems #####
	${STELLA_SRC_DIR}/Stella/Systems/Scene/*cpp
	${STELLA_SRC_DIR}/Stella/Systems/ModelManager/*cpp
	${STELLA_SRC_DIR}/Stella/Systems/SpriteManager/*cpp
	${STELLA_SRC_DIR}/Stella/Systems/DeltaTime/*cpp
	${STELLA_SRC_DIR}/Stella/Systems/Component/*cpp

	##### Renderer #####
	${STELLA_SRC_DIR}/Stella/Renderer/Texture/*cpp
	${STELLA_SRC_DIR}/Stella/Renderer/Shader/*cpp
	${STELLA_SRC_DIR}/Stella/Renderer/QuadRenderer/*cpp
	${STELLA_SRC_DIR}/Stella/Renderer/MeshRenderer/*cpp
	${STELLA_SRC_DIR}/Stella/Renderer/SpriteRenderer/*cpp
	${STELLA_SRC_DIR}/Stella/Renderer/Framebuffer/*cpp
	${STELLA_SRC_DIR}/Stella/Renderer/Camera/*cpp
	${STELLA_SRC_DIR}/Stella/Renderer/Sprite/*cpp
)

add_library(
	${PROJECT_NAME}
	${SOURCES}
	${GLAD_DIR}/src/glad.c
	${TINY_OBJ_DIR}/tiny_obj_loader.cc
	#${TINY_OBJ_DIR}/loader_example.cc
)

##### INCLUDE DIRS
target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	${STELLA_SRC_DIR}
	${GLAD_DIR}/include
	${GLM_DIR}
	${ENTT_DIR}
	${TINY_OBJ_DIR}
	${STB_DIR}
	${BOX2D_DIR}/include
	${OPEN_CL_DIR}/include
)

target_link_libraries(
	${PROJECT_NAME}
	glfw
	box2d
	"-framework OpenCL"
)

set_target_properties(
	${PROJECT_NAME}
    PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_compile_definitions(STELLA_SRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}" STELLA_SRC_DIR_SET)