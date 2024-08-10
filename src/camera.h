#pragma once

#include "math/vector.h"

#include <memory> // shared_ptr

namespace ciel {

class Camera
{
public:
    Camera()
    : mFov(60.0)
    , mAspectRatio(3.0 / 2.0)
    , mNear(0.0)
    , mFar(0.0)
    {
        setEyeViewUp(Vector(0, 0, 1), Vector(0, 0, -1), Vector(0, 1, 0));
    }

    using Ptr = std::shared_ptr<Camera>;
    using ConstPtr = std::shared_ptr<const Camera>;
    static Ptr      create() { return std::make_shared<Camera>(); }
    static ConstPtr createConst() { return std::make_shared<const Camera>(); }

    void setEyeViewUp(const Vector &eye, const Vector &view, const Vector &up)
    {
        mPosition = eye;
        mView = view.unitvector();
        mUp = (up - (up * mView) * mView).unitvector();
        mRight = (mView ^ mUp).unitvector();
    }
    const Vector &eye() const { return mPosition; }
    const Vector &view() const { return mView; }
    const Vector &up() const { return mUp; }

    // view direction of a pixel at the fractional position x,y.
    // Nominally 0 <= x <= 1 and 0 <= y <= 1 for the primary fov,
    // but the values can extend beyond that
    const Vector view(const float x, const float y)
    {
        float xx = (2.0 * x - 1.0) * htanfov;
        float yy = (2.0 * y - 1.0) * vtanfov;
        return (mUp * yy + mRight * xx + mView).unitvector();
    }

    void setFov(const float fov)
    {
        mFov = fov;
        htanfov = tan(mFov * 0.5 * M_PI / 180.0);
        vtanfov = htanfov / mAspectRatio;
    }

    float fov() const { return mFov; }

    void setAspectRatio(const float ar)
    {
        mAspectRatio = ar;
        vtanfov = htanfov / mAspectRatio;
    }
    float aspectRatio() const { return mAspectRatio; }

    void  setNearPlane(const float n) { mNear = n; }
    float nearPlane() const { return mNear; }

    void  setFarPlane(const float n) { mFar = n; }
    float farPlane() const { return mFar; }

private:
    float mFov, mAspectRatio;
    float htanfov, vtanfov;
    float mNear, mFar;

    Vector mPosition;
    Vector mRight, mUp, mView;
};

} // namespace ciel