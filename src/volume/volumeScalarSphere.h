#pragma once

#include "math/vector.h"
#include "volumeBase.h"

namespace ciel {

class VolumeScalarSphere : public VolumeScalar
{
public:
    VolumeScalarSphere(const Vector& tCenter, float tRadius)
    : m_center(tCenter)
    , m_radius(tRadius)
    {
    }

    using Ptr = std::shared_ptr<VolumeScalarSphere>;
    using ConstPtr = std::shared_ptr<const VolumeScalarSphere>;

    float eval(const Vector& p) const override
    {
        return (m_radius - length(p - m_center));
    }
    Vector dxdy(const Vector& p) const override
    {
        return -1.f * (p - m_center) / Vector(p - m_center).magnitude();
    }

    static Ptr create(const Vector& center, float radius)
    {
        return std::make_shared<VolumeScalarSphere>(center, radius);
    }
    static ConstPtr createConst(const Vector& center, float radius)
    {
        return std::make_shared<const VolumeScalarSphere>(center, radius);
    }

    Vector center() const { return m_center; }
    float  radius() const { return m_radius; }
    void   setCenter(const Vector& center) { m_center = center; }
    void   setRadius(float radius) { m_radius = radius; }

private:
    Vector m_center;
    float  m_radius;
};

} // namespace ciel
