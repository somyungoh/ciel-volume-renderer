#pragma once

namespace ciel {

struct RenderSetting
{
    // Size
    unsigned render_w{0};
    unsigned render_h{0};

    // Returns size of the pixmap.
    // Currently: width * height * 3(rgb)
    unsigned pixmapSize() const { return render_w * render_h * 3; }
};

} // namespace ciel
