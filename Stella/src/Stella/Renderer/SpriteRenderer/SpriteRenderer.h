#pragma once

#include <glm/glm.hpp>

namespace Stella
{

class Camera;
class Model;
class Sprite;
    
class SpriteRenderer
{

public:

    SpriteRenderer();
    ~SpriteRenderer();

    void start(Camera * camera);
    void render_sprite(const glm::mat4 & transform, Sprite * p_sprite);
    void end();

private:

    Camera * m_pCamera;
    Model * m_pBaseQuad;

};

} // namespace Stella
