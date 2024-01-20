#include "renderer.h"

namespace ciel {

void Renderer::render(const RenderSetting &setting)
{
    // Occupy vector storage
    m_pixmap.reserve(setting.pixmapSize());

    // ----------------------------------------
    //  Rendering
    // ----------------------------------------
    for (size_t i = 0; i < setting.pixmapSize(); i++)
        m_pixmap[i] = 0.f;

    printf("[ciel][render] Rendering complete.\n");
}

} // namespace ciel