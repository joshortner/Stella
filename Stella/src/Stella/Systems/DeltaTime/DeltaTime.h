#pragma once 

namespace Stella
{

struct DeltaTime
{
    float m_dt;
    double m_time;

    DeltaTime();
    void tick();

    operator float() const;
};
    
} // namespace Stella
