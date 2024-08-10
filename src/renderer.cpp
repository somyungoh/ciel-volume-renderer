#include "renderer.h"
#include "math/color.h"
#include "math/vector.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <print>

constexpr float TEMP_DS = 0.001;
constexpr float TEMP_DLS = 0.00025;
constexpr int   TEMP_K = 10; // 600         // ** This one is critical

namespace ciel {

void Renderer::Render(const RenderSetting& setting)
{
    // Init Scene
    if (mScene == nullptr) {
        mScene = Scene::create();
    }
    mScene->init(setting.render_w, setting.render_h);

    // Occupy vector storage
    mPixmap.resize(setting.pixmapSize());

    // total number of steps
    const float nSteps = (mScene->getCamera()->farPlane() -
                          mScene->getCamera()->nearPlane()) /
                         TEMP_DS; // total sample N

    std::println("[ciel][render] Start Rendering...");
    auto startTime = std::chrono::system_clock::now();

    // Render!
    for (size_t j = 0; j < setting.render_h; j++) {
        for (size_t i = 0; i < setting.render_w; i++) {
            const Vector ray = mScene->getCamera()->view(
                (float)i / setting.render_w, (float)j / setting.render_h);
#ifdef _OPENMP
            const Color c = RayMarchOMP(ray, nSteps, setting);
#else
            const Color c = RayMarch(ray, nSteps, setting);
#endif // _OPENMP
            mPixmap[(j * setting.render_w + i) * 4 + 0] = c.X();
            mPixmap[(j * setting.render_w + i) * 4 + 1] = c.Y();
            mPixmap[(j * setting.render_w + i) * 4 + 2] = c.Z();
            mPixmap[(j * setting.render_w + i) * 4 + 3] = c.W();
        }
    }

    auto endTime = std::chrono::system_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                           endTime - startTime)
                           .count();
    std::cout << "[ciel][render] Rendering complete. Elapsed: "
              << elapsedTime / 1000.0 << " seconds" << std::endl;
}

// ------------------------------------------------
//  Where "Volume Rendering" happens
// ------------------------------------------------
Color Renderer::RayMarch(const Vector&        ray,
                         const size_t         nSteps,
                         const RenderSetting& setting)
{
    Vector xp = mScene->getCamera()->eye() +
                ray * mScene->getCamera()->nearPlane(); // first position
    Color L(0, 0, 0, 1); // color attenuated by length (init. black)
    float T = 1;         // total transmissity

    // Iteratively running over the steps [0, 1 ... nSteps]
    // solve Kajuya's Rendering Equation:
    //    [INTEGRAL](s) * K * Color(P) * Density(P) * Transmissity(P)
    for (size_t j = 0; j < nSteps; j++) {
        // prepare variables
        float density = 0.0;
        Color cx;

        // 1. Compute X(p,s)
        xp += ray * TEMP_DS;

        // 2. Density(X)    * Important Step!
        mScene->eval(xp, density, cx);
        density = density < std::numeric_limits<float>::epsilon() // masking
                      ? 0
                      : 1; // density;
        // try to stay exp(K), 0 < K < ds
        const float dt = exp(-1.f * TEMP_K * TEMP_DLS * density);

        // 3. Color(X)
        if (density > 0) {
            L += cx * (1 - dt) * T;
        }

        // 4. Transmissity
        T *= dt;
    }
    L[3] = 1 - T;
    return L; // return final L (color)
}

// Parallel (OpenMP) version of RayMarch()
Color Renderer::RayMarchOMP(const Vector&        ray,
                            const size_t         nSteps,
                            const RenderSetting& setting)
{
    const Vector pStart = mScene->getCamera()->eye() +
                          ray * mScene->getCamera()->nearPlane();

    std::vector<std::pair<Color, float>> record(nSteps);

#pragma omp parallel for
    for (size_t j = 0; j < nSteps; j++) {
        float density = 0.0;
        Color cx;

        // 1. Compute X(p,s)
        const Vector xp = pStart + ray * j * TEMP_DS;

        // 2. Density(X)    * Important Step!
        mScene->eval(xp, density, cx);
        density = density < std::numeric_limits<float>::epsilon() // masking
                      ? 0
                      : 1;
        // try to stay exp(K), 0 < K < ds
        const float dt = exp(-1.f * TEMP_K * TEMP_DLS * density);

        record[j] = std::pair<Color, float>(cx, dt);
    }

    Color L(0, 0, 0, 1); // color attenuated by length (init. black)
    float T = 1;         // total transmissity

    // This region cannot be parallelized
    for (size_t j = 0; j < nSteps; j++) {
        const Color cx = record[j].first;
        const float dt = record[j].second;

        // 3. Color(X)
        if (dt < 1) {
            L += cx * (1 - dt) * T;
        }

        // 4. Transmissity
        T *= dt;
    }
    L[3] = 1 - T;
    return L; // return final L (color)
}

} // namespace ciel