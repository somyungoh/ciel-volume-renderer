#pragma once

#include "math/vector.h"
#include "volumeBase.h"

#include <algorithm>
#include <memory>

namespace ciel {

class VolumeScalarBox : public VolumeScalar
{
public:
    VolumeScalarBox(const Vector& center, const Vector& bound, float exponent)
    : m_center(center)
    , m_bound(bound)
    , m_exp(exponent)
    {
    }

    using Ptr = std::shared_ptr<VolumeScalarBox>;
    using ConstPtr = std::shared_ptr<const VolumeScalarBox>;

    volumeDataType eval(const Vector& p) const override
    {
        const Vector q = abs(p - m_center) - m_bound + m_exp;

        // The original equation equals d == 0 to be "inside", but our renderer
        // is (d > 0) == "inside" so we do a bit of hack.
        //    return length(max(q, 0)) - m_exp;
        volumeDataType sign = length(max(q, 0)) - m_exp;
        return sign < std::numeric_limits<float>::epsilon() ? 1 : -sign;
    }

    [[deprecated("Not Implemented!")]] volumeDxDyType
    dxdy([[maybe_unused]] const Vector& p) const override
    {
        return volumeDxDyType{};
    }

    static Ptr create(const Vector& center, const Vector& bound, float exponent)
    {
        return std::make_shared<VolumeScalarBox>(center, bound, exponent);
    }
    static ConstPtr
    createConst(const Vector& center, const Vector& bound, float exponent)
    {
        return std::make_shared<const VolumeScalarBox>(center, bound, exponent);
    }

    Vector Center() const { return m_center; }
    Vector Bound() const { return m_bound; }
    float  Exp() const { return m_exp; }
    void   SetCenter(const Vector& center) { m_center = center; }
    void   SetBound(const Vector& bound) { m_bound = bound; }
    void   SetExp(float exp) { m_exp = exp; }

private:
    Vector m_center;
    Vector m_bound;
    float  m_exp; // determines the edge curvature (base = 1)
};

} // namespace ciel
