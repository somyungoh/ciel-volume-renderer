#pragma once

namespace ciel {

struct RenderSetting
{
    // Size
    unsigned render_w{0};
    unsigned render_h{0};

    // Returns size of the pixmap.
    // Currently: width * height * 4(rgba)
    unsigned pixmapSize() const { return render_w * render_h * 4; }
};

} // namespace ciel
