#pragma once

#include "math/vector.h"
#include "volumeBase.h"

namespace ciel {

class VolumeScalarTorus : public VolumeScalar
{
public:
    VolumeScalarTorus(const Vector& tCenter,
                      const Vector& tNormal,
                      float         tRadius1,
                      float         tRadius2)
    : m_center(tCenter)
    , m_normal(tNormal)
    , m_radius1(tRadius1)
    , m_radius2(tRadius2)
    {
    }

    using Ptr = std::shared_ptr<VolumeScalarTorus>;
    using ConstPtr = std::shared_ptr<const VolumeScalarTorus>;

    float eval(const Vector& p) const override
    {
        const Vector x = p - m_center;
        const Vector xp = x - (x * m_normal) * m_normal;

        return (4.0 * m_radius1 * m_radius1 * (xp * xp)) -
               pow((x * x) + m_radius1 * m_radius1 - m_radius2 * m_radius2, 2);
    }
    [[deprecated("Not Implemented!")]] Vector
    dxdy([[maybe_unused]] const Vector& p) const override
    {
        return Vector{};
    }

    static Ptr create(const Vector& tCenter,
                      const Vector& tNormal,
                      float         tRadius1,
                      float         tRadius2)
    {
        return std::make_shared<VolumeScalarTorus>(
            tCenter, tNormal, tRadius1, tRadius2);
    }
    static ConstPtr createConst(const Vector& tCenter,
                                const Vector& tNormal,
                                float         tRadius1,
                                float         tRadius2)
    {
        return std::make_shared<const VolumeScalarTorus>(
            tCenter, tNormal, tRadius1, tRadius2);
    }

    Vector center() const { return m_center; }
    Vector normal() const { return m_normal; }
    void   setCenter(const Vector& center) { m_center = center; }
    void   setNormal(const Vector& normal) { m_normal = normal; }
    void   setRadius1(float radius) { m_radius1 = radius; }
    void   setRadius2(float radius) { m_radius2 = radius; }

private:
    Vector m_center;
    Vector m_normal;
    float  m_radius1;
    float  m_radius2;
};

} // namespace ciel
