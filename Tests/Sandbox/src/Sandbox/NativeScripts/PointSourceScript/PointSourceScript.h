#pragma once

#include "Stella.h"

using namespace Stella;

class PointSourceScript : NativeScriptBase
{

	static const uint32_t FOV = 360;
	static const uint32_t MAX_RAY_DIST = 100;

public:

	~PointSourceScript();

    virtual void initialize() override;
	virtual void on_update() override;
    virtual void destroy() override;

private:

	std::vector<Ray2D> m_sourceRays;
	uint32_t m_rayDensity = 1; // "rays per angle"

};