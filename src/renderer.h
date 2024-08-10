#pragma once

#include "renderSetting.h"
#include "scene.h"

#include <stdint.h>
#include <vector>

namespace ciel {

class Renderer
{
public:
    // Main render logic
    void Render(const RenderSetting &setting);

    [[nodiscard]] Color RayMarch(const Vector        &ray,
                                 const size_t         nSteps,
                                 const RenderSetting &setting);
    [[nodiscard]] Color RayMarchOMP(const Vector        &ray,
                                    const size_t         nSteps,
                                    const RenderSetting &setting);

    // Returns a copy of last rendered pixels
    [[nodiscard]] std::vector<float> getLastRender() const
    {
        return std::vector<float>(mPixmap);
    }

private:
    Scene::Ptr         mScene;
    std::vector<float> mPixmap;
};

} // namespace ciel