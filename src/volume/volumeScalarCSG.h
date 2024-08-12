#pragma once

// -------------------------------------------------------
//
//  A collection of constructive solid geometry (CSG)
//  operations that can be performend on scalar volumes.
//
// -------------------------------------------------------

#include "volumeBase.h"

#include <algorithm>

namespace ciel {

class VolumeScalarUnion : public VolumeScalar
{
public:
    VolumeScalarUnion(VolumeScalar::Ptr tField1, VolumeScalar::Ptr tField2)
    : mField1(tField1)
    , mField2(tField2) {};

    float eval(const Vector& p) const override
    {
        return std::max(mField1->eval(p), mField2->eval(p));
    }

private:
    const VolumeScalar::Ptr mField1;
    const VolumeScalar::Ptr mField2;
};

class VolumeScalarIntersection : public VolumeScalar
{
public:
    VolumeScalarIntersection(VolumeScalar::Ptr tField1,
                             VolumeScalar::Ptr tField2)
    : mField1(tField1)
    , mField2(tField2) {};

    float eval(const Vector& p) const override
    {
        return std::min(mField1->eval(p), mField2->eval(p));
    }

private:
    const VolumeScalar::Ptr mField1;
    const VolumeScalar::Ptr mField2;
};

class VolumeScalarCutout : public VolumeScalar
{
public:
    VolumeScalarCutout(VolumeScalar::Ptr tField1, VolumeScalar::Ptr tField2)
    : mField1(tField1)
    , mField2(tField2) {};

    float eval(const Vector& p) const override
    {
        return std::min(mField1->eval(p), -1.f * mField2->eval(p));
    }

private:
    const VolumeScalar::Ptr mField1;
    const VolumeScalar::Ptr mField2;
};

class VolumeScalarShell : public VolumeScalar
{
public:
    VolumeScalarShell(VolumeScalar::Ptr tField1, float tThickness);

    float eval(const Vector& p) const override
    {
        return std::min((mField->eval(p) + mThickness / 2.f),
                        -1.f * (mField->eval(p) - mThickness / 2.f));
    }

private:
    const VolumeScalar::Ptr mField;
    const float             mThickness;
};

} // namespace ciel
