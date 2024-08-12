#include "renderer.h"
#include "math/color.h"
#include "math/vector.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <print>

namespace ciel {

void Renderer::Render(const RenderSetting& setting)
{
    // Init Scene
    if (m_scene == nullptr) {
        m_scene = Scene::create();
    }
    m_scene->init(setting.renderW, setting.renderH);

    // Occupy vector storage
    m_pixmap.resize(setting.pixmapSize());

    // total number of steps
    const float nSteps = (m_scene->getCamera()->farPlane() -
                          m_scene->getCamera()->nearPlane()) /
                         setting.rayDt; // total sample N

    std::println("[ciel][render] Start Rendering...");
    auto startTime = std::chrono::system_clock::now();

    // Render!
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(setting, nSteps, m_pixmap)
#endif // _OPENMP
    for (size_t j = 0; j < setting.renderH; j++) {
        for (size_t i = 0; i < setting.renderW; i++) {
            const Vector ray = m_scene->getCamera()->view(
                (float)i / setting.renderW, (float)j / setting.renderH);

            const Color c = RayMarch(ray, nSteps, setting);

            m_pixmap[(j * setting.renderW + i) * 4 + 0] = c.X();
            m_pixmap[(j * setting.renderW + i) * 4 + 1] = c.Y();
            m_pixmap[(j * setting.renderW + i) * 4 + 2] = c.Z();
            m_pixmap[(j * setting.renderW + i) * 4 + 3] = c.W();
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
    Vector xp = m_scene->getCamera()->eye() +
                ray * m_scene->getCamera()->nearPlane(); // first position
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
        xp += ray * setting.rayDt;

        // 2. Density(X)    * Important Step!
        m_scene->eval(xp, density, cx);
        density = density < std::numeric_limits<float>::epsilon() // masking
                      ? 0
                      : 1; // density;
        // try to stay exp(K), 0 < K < ds
        const float dt = exp(-1.f * setting.expK * density);

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
    const Vector pStart = m_scene->getCamera()->eye() +
                          ray * m_scene->getCamera()->nearPlane();

    std::vector<std::pair<Color, float>> record(nSteps);

#pragma omp parallel for default(none),                                        \
    shared(ray, nSteps, setting, pStart, record), schedule(static)
    for (size_t j = 0; j < nSteps; j++) {
        float density = 0.0;
        Color cx;

        // 1. Compute X(p,s)
        const Vector xp = pStart + ray * j * setting.rayDt;

        // 2. Density(X)    * Important Step!
        m_scene->eval(xp, density, cx);
        density = density < std::numeric_limits<float>::epsilon() // masking
                      ? 0
                      : 1;
        // try to stay exp(K), 0 < K < ds
        const float dt = exp(-1.f * setting.expK * density);

        record[j] = std::pair<Color, float>(cx * (1 - dt), dt);
    }

    Color L(0, 0, 0, 1); // color attenuated by length (init. black)
    float T = 1;         // total transmissity

    // This region cannot be parallelized
    for (size_t j = 0; j < nSteps; j++) {
        const Color cx = record[j].first;
        const float dt = record[j].second;

        // 3. Color(X)
        if (dt < 1) {
            L += cx * T;
        }

        // 4. Transmissity
        T *= dt;
    }
    L[3] = 1 - T;
    return L; // return final L (color)
}

} // namespace ciel