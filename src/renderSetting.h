#pragma once

namespace ciel {

struct RenderSetting
{
    // Image size
    unsigned renderW{800};
    unsigned renderH{600};

    // Render parameters
    float rayDt{0.01}; // Raymarch step size
    float expK{0.02};  // What is this?

    // Returns size of the pixmap.
    // Currently: width * height * 4(rgba)
    unsigned pixmapSize() const { return renderW * renderH * 4; }
};

} // namespace ciel
