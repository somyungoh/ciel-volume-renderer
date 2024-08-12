#pragma once

#include "math/vector.h"
#include "volumeBase.h"

namespace ciel {

class VolumeScalarEllipse : public VolumeScalar
{
public:
    VolumeScalarEllipse(const Vector& tCenter,
                        const Vector& tStretch,
                        float         tRadius1,
                        float         tRadius2)
    : m_center(tCenter)
    , m_stretch(tStretch)
    , m_radius1(tRadius1)
    , m_radius2(tRadius2)
    {
    }

    using Ptr = std::shared_ptr<VolumeScalarEllipse>;
    using ConstPtr = std::shared_ptr<const VolumeScalarEllipse>;

    float eval(const Vector& p) const override
    {
        Vector x = p - m_center;
        float  Z = x * m_stretch;
        Vector xp = x - Z * m_stretch;

        return (1 - (Z * Z) / m_radius1 * m_radius1 -
                (xp * xp) / m_radius2 * m_radius2);
    }
    [[deprecated("Not Implemented!")]] Vector
    dxdy([[maybe_unused]] const Vector& p) const override
    {
        return Vector{};
    }

    static Ptr create(const Vector& tCenter,
                      const Vector& tStretch,
                      float         tRadius1,
                      float         tRadius2)
    {
        return std::make_shared<VolumeScalarEllipse>(
            tCenter, tStretch, tRadius1, tRadius2);
    }
    static ConstPtr createConst(const Vector& tCenter,
                                const Vector& tStretch,
                                float         tRadius1,
                                float         tRadius2)
    {
        return std::make_shared<const VolumeScalarEllipse>(
            tCenter, tStretch, tRadius1, tRadius2);
    }

    Vector center() const { return m_center; }
    Vector stretch() const { return m_stretch; }
    void   setCenter(const Vector& center) { m_center = center; }
    void   setStretch(const Vector& stretch) { m_stretch = stretch; }
    void   setRadius1(float radius) { m_radius1 = radius; }
    void   setRadius2(float radius) { m_radius2 = radius; }

private:
    Vector m_center;
    Vector m_stretch;
    float  m_radius1;
    float  m_radius2;
};

} // namespace ciel
