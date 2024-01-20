#pragma once

#include "renderSetting.h"

#include <stdint.h>
#include <vector>

namespace ciel {

class Renderer
{
public:
    // Main render logic
    void render(const RenderSetting &setting);

    // Returns a copy of last rendered pixels
    [[nodiscard]] std::vector<float> getLastRender() const
    {
        return std::vector<float>(m_pixmap);
    }

private:
    std::vector<float> m_pixmap;
};

} // namespace ciel