#pragma once

#include "renderSetting.h"

#include <GLFW/glfw3.h>
#include <memory>
#include <vector>

namespace ciel {

class Renderer;

class CielApp
{
public:
    CielApp();
    ~CielApp();

    void run();
    void init(const RenderSetting &setting)
    {
        initGLFW(setting.render_w, setting.render_h);
    }

private:
    void initGLFW(int width, int height);
    void mainLoop();
    void cleanup();

private:
    std::unique_ptr<Renderer> mRenderer;

    // GLFW Window
    GLFWwindow *m_window;
};

} // namespace ciel
