#pragma once

#include "camera.h"
#include "volume/volumeBase.h"

#include <vector>

namespace ciel {

class Color;
class Vector;

class Scene
{
public:
    using Ptr = std::shared_ptr<Scene>;
    using ConstPtr = std::shared_ptr<const Scene>;
    static Ptr      create() { return std::make_shared<Scene>(); }
    static ConstPtr createConst() { return std::make_shared<const Scene>(); }

    // Main eval funtion
    void eval(const Vector &p, float &outDensity, Color &outColor);

    // Scene initialization method
    void init(int imgX, int imgY);
    void update();
    bool AABBCheck(const Vector &origin, const Vector &direction) const;

    // getter, setters
    Camera::Ptr getCamera() { return mCam; }
    // Shape::Ptr  getShape(int i)
    // {
    //     return mShapes[i];
    // } // might not need these anymore
    // inline Light::Ptr getLight(int i) { return mLights[i]; }
    // inline int        shapeSize() const { return mShapes.size(); }
    // inline int        lightSize() const { return mLights.size(); }

    // inline void addShape(const Shape::Ptr &object)
    // {
    //     mShapes.push_back(object);
    // }
    inline void incFrameCount() { mFrameCount++; }

    // camera control
    // inline void moveIn(float ds) { mCam->moveIn(ds); }
    // inline void moveOut(float ds) { mCam->moveOut(ds); }
    // inline void rotate(float da) { mCam->rotate(da); }

private:
    // main camera
    Camera::Ptr mCam;

    // current frame count
    int mFrameCount;

    // vector that stores vector
    std::vector<VolumeScalar::Ptr> mVolumes;
    // std::vector<Light::Ptr> mLights;

    // local initialize methods
    void initCamera(int Nx, int Ny); // init camera
    void initVolume();               // do modeling
    void initLight();                // set lights
    void initMap();                  // set grids
};

} // namespace ciel
