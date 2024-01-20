#pragma once

#include "../math/vector.h"
#include "volumeBase.h"

#include <memory>

namespace ciel {

class VolumeScalarSphere : public VolumeScalar
{
public:
    VolumeScalarSphere() = default;
    VolumeScalarSphere(const Vector &center, float radius)
    : mCenter(center)
    , mRadius(radius)
    {
    }
    virtual ~VolumeScalarSphere() = default;

    using Ptr = std::shared_ptr<VolumeScalarSphere>;
    using ConstPtr = std::shared_ptr<const VolumeScalarSphere>;

    volumeDataType eval(const Vector &p) const override
    {
        return (mRadius - Vector(p - mCenter).magnitude());
    }
    volumeDxDyType dxdy(const Vector &p) const override
    {
        return -1.f * (p - mCenter) / Vector(p - mCenter).magnitude();
    }

    static Ptr create(const Vector &center, float radius)
    {
        return std::make_shared<VolumeScalarSphere>(center, radius);
    }
    static ConstPtr createConst(const Vector &center, float radius)
    {
        return std::make_shared<const VolumeScalarSphere>(center, radius);
    }

    Vector center() const { return mCenter; }
    float  radius() const { return mRadius; }
    void   setCenter(const Vector &center) { mCenter = center; }
    void   setRadius(float radius) { mRadius = radius; }

private:
    Vector mCenter;
    float  mRadius;
};

} // namespace ciel
