#include "DeltaTime.h"

#include <GLFW/glfw3.h>

namespace Stella
{

DeltaTime::DeltaTime() : m_dt(1.0f / 60.0f) 
{
    m_time = glfwGetTime();
}

void DeltaTime::tick()
{
    float prev_frame = m_time;
    m_time = glfwGetTime();
    m_dt = m_time - prev_frame;
}

DeltaTime::operator float() const
{
    return m_dt;
}
    
} // namespace Stella
