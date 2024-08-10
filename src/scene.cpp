/**********************************************************************************
 *
 *        scene.cpp
 *
 *        A scene class that stores all the Shapes
 *        OBJECT MODELING is done in this class
 *
 *
 *        ***** HOW TO CREATE A SCENE *****
 *
 *        1) setLight():
 *            initialize all the light is going to be used.
 *            setting DSM will be done at setMap() after all the
 *            ScalarFields are set.
 *
 *        2) setModel():
 *            All scalar fields(implicit shpaes, noise..) are set here.
 *            CSG modeling, grid generation is also done in this step.
 *
 *        3) setMap():
 *            Special grid generation is done in this step.
 *            Including: WISP, DSM, Color.
 *
 *
 *        ******        USING COLOR MAP        ******
 *
 *        colorMap variable (master ColorField) is very useful when
 *        setting up colors. It automatically generates by taking ALL Shapes,
 *        -generate(), then adds up my taking a single shape -mix().
 *        Since now master color map stores individual color data, when all the
 *        scalar field is stamped to a single grid, it can still get the color
 *        in individual Shapes.
 *
 *
 *        ******        DEVNOTEs        ******
 *
 *        TODO:
 *        - AABB is always at fixed point even though they are transformed.
 *        - Also, what if gridfield is unioned with other objects..? typecasting
 *          would not work.
 *        - ColorField is not working
 *
 ************************************************************************************/

#include "scene.h"

#include "math/color.h"
#include "math/vector.h"
#include "volume/volumeScalarSphere.h"

namespace ciel {

// main color field
// ColorField::Ptr colorMap;

// collect eval() data from all the fields
// then return to the renderer
void Scene::eval(const Vector& p, float& outDensity, Color& outColor)
{
    // initialize
    Color comp(0, 0, 0, 0);
    float density = 0.0;

    // collect eval()
    for (size_t i = 0; i < mVolumes.size(); i++) {
        // collect density
        const float val = mVolumes[i]->eval(p);
        density += val < 0 ? 0 : val;

        // collect color
        // if (colorMap &&
        //     std::dynamic_pointer_cast<ColorGrid>(colorMap)->isReady())
        //     comp += colorMap->eval(p);
        // else
        //     comp += mVolumes[i]->Color() / (float)mVolumes.size();
    }

    // return back
    outDensity = density;
    outColor = Color(1, 1, 1, 1); // comp;
};

// ------------------------------------------------
//  Where "Volume Modeling" happens
// ------------------------------------------------
void Scene::initVolume()
{
    VolumeScalar::Ptr sphere1 = VolumeScalarSphere::create(Vector(-0.5, 0, 0),
                                                           0.5);
    VolumeScalar::Ptr sphere2 = VolumeScalarSphere::create(Vector(0.5, 0, 0),
                                                           0.5);
    mVolumes.push_back(sphere1);
    mVolumes.push_back(sphere2);
};

// Axis Aligned Bounding Box(AABB) Checking
// returns true if box is hit, false if no intersection
// precondition: origin / direction of the ray
// bool Scene::AABBCheck(const Vector &o, const Vector &d) const
// {
//     // scalar grids
//     for (size_t i = 0; i < mVolumes.size(); i++) {
//         if (std::dynamic_pointer_cast<const ScalarGrid>(
//                 mVolumes[i]->getScalarField())) {
//             return std::dynamic_pointer_cast<const ScalarGrid>(
//                        mVolumes[i]->getScalarField())
//                 ->checkAABB(o, d);
//         }
//         // TODO::vector grids
//         // else if()
//     }
// }

void Scene::init(int imgX, int imgY)
{
    initCamera(imgX, imgY);
    // setLight();
    initVolume();
    // setMap();
}

void Scene::update()
{
    mVolumes.clear();
    // mLights.clear();

    initLight();
    initVolume();
    initMap();
}

// sub method of init()
// setup camera model
void Scene::initCamera(int Nx, int Ny)
{
    // constexpr Vector  cameraOrigin = Vector(0, 1.8, -5);
    constexpr Vector camOrigin(0, 0, -5);
    constexpr Vector camUp(0, 1, 0);
    constexpr float  camFov = 40.f;
    Vector           camView = Vector(0, 0, 0) - camOrigin;
    float            camAspectRatio = (float)Nx / Ny;

    // create camera
    mCam = Camera::create();
    mCam->setEyeViewUp(camOrigin, camView, camUp);
    mCam->setAspectRatio(camAspectRatio);
    mCam->setFov(camFov);
    mCam->setNearPlane(0.1);
    mCam->setFarPlane(10.0);
}

// sub method of init()
// setup mLights
void Scene::initLight()
{
    constexpr Color pink_cocktail(1, 0.305, 0.313, 1);
    constexpr Color heart_orange(0.985, 0.568, 0.227, 1);
    constexpr Color above_yellow(0.971, 0.831, 0.137, 1);
    constexpr Color blue_green(0.27, 0.678, 0.658, 1);

    constexpr Color love_red(0.815, 0.0941, 0.2117, 1);
    constexpr Color love_orange(0.972, 0.349, 0.192, 1);
    constexpr Color love_yellow(0.929, 0.349, 0.725, 1);
    constexpr Color love_green(0.639, 0.662, 0.725, 1);
    constexpr Color love_blue(0, 0.6, 0.725, 1);
    constexpr Color love_purple(0.364, 0.2549, 0.341, 1);
}

void Scene::initMap() {}

} // namespace ciel
