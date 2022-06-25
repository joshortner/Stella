#pragma once

#include <glm/glm.hpp>

#include <stdint.h>

#include <vector>

namespace Stella
{

class Camera;
class Sprite;
class Shader;

struct QuadVertex 
{
    glm::vec3 m_position;
    glm::vec4 m_color;
	glm::vec2 m_uv;
	float m_textureId;
};


struct QuadBatchRenderState
{
static const uint32_t MAX_QUADS = 10000;

	Shader * m_pShader;
	Camera * m_pCamera;

	uint32_t m_vao;
	uint32_t m_vbo;
	uint32_t m_ebo;

	uint32_t m_indexCount;
	QuadVertex m_vertexBufferBase[MAX_QUADS * 4];
    QuadVertex * m_vertexBufferPtr;

	uint32_t m_textureSlotIdx = 1; // TODO: Unit zero doesn't work?
	std::vector<uint32_t> m_textureSlots;

	QuadBatchRenderState();

	void start(Camera * p_cam);
	void end();
	void push_quad(const glm::vec3 & tr, const glm::vec3 & br, const glm::vec3 & bl, const glm::vec3 & tl, const glm::vec4 & color);
	void push_sprite(const glm::mat4 & transform, Sprite * p_sprite);

private:

	void flush();
};

class QuadRenderer
{

	static const uint32_t MAX_QUADS = 100;

public:

	QuadRenderer();
	~QuadRenderer();

	void start(Camera * p_cam);
	void end();
	void push_quad(const glm::vec3 & tr, const glm::vec3 & br, const glm::vec3 & bl, const glm::vec3 & tl, const glm::vec4 & color);
	void push_sprite(const glm::mat4 & transform, Sprite * p_sprite);

private:

	//void flush();

	Camera * m_pCamera;

	uint32_t m_vao;
	uint32_t m_vbo;
	uint32_t m_ebo;

	uint32_t m_indexCount;
	QuadVertex m_vertexBufferBase[MAX_QUADS * 4];
    QuadVertex * m_vertexBufferPtr;

	uint32_t m_textureSlotIdx = 1;
	uint32_t m_textureSlots[8];

	QuadBatchRenderState m_defaultRenderState;

};

} // namespace Stella
