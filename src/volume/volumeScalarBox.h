#pragma once

#include "math/vector.h"
#include "volumeBase.h"

#include <memory>

namespace ciel {

class VolumeScalarBox : public VolumeScalar
{
public:
    VolumeScalarBox(const Vector& center, float radius, float exponent)
    : mCenter(center)
    , mRadius(radius)
    , mExp(exponent)
    {
    }

    using Ptr = std::shared_ptr<VolumeScalarBox>;
    using ConstPtr = std::shared_ptr<const VolumeScalarBox>;

    volumeDataType eval(const Vector& p) const override
    {
        const Vector x = p - mCenter;
        return pow(mRadius, mExp) - pow(x.X(), mExp) - pow(x.Y(), mExp) -
               pow(x.Z(), mExp);
    }
    [[deprecated("Not Implemented!")]] volumeDxDyType
    dxdy([[maybe_unused]] const Vector& p) const override
    {
        return volumeDxDyType{};
    }

    static Ptr create(const Vector& center, float radius, float exponent)
    {
        return std::make_shared<VolumeScalarBox>(center, radius, exponent);
    }
    static ConstPtr
    createConst(const Vector& center, float radius, float exponent)
    {
        return std::make_shared<const VolumeScalarBox>(
            center, radius, exponent);
    }

    Vector Center() const { return mCenter; }
    float  Radius() const { return mRadius; }
    float  Exp() const { return mExp; }
    void   SetCenter(const Vector& center) { mCenter = center; }
    void   SetRadius(float radius) { mRadius = radius; }
    void   SetExp(float exp) { mExp = exp; }

private:
    Vector mCenter;
    float  mRadius;
    float  mExp; // determines the edge curvature (base = 1)
};

} // namespace ciel
