#pragma once

#include "renderSetting.h"
#include "renderer.h"

#include <GLFW/glfw3.h>
#include <memory>
#include <vector>

namespace ciel {

class Renderer;

class CielApp
{
public:
    void run();
    void init(int windowW, int windowH);

    void                 setNeedRender(bool need) { m_needRender = need; }
    const RenderSetting& getRenderSetting() { return m_renderSetting; }
    void                 setRenderSetting(const RenderSetting& newSetting)
    {
        m_renderSetting = newSetting;
    }

private:
    void initGLFW(int width, int height);
    void initImGui();

    // openGL stuff
    void createGLShader(unsigned int& outShaderProgram);
    void createGLQuad(void*         textureData,
                      unsigned int& outVBO,
                      unsigned int& outVAO,
                      unsigned int& outEBO,
                      unsigned int& outTextureID);

    void mainLoop();

    void cleanup();

private:
    std::unique_ptr<Renderer> m_renderer;
    RenderSetting             m_renderSetting;

    // GLFW Window
    GLFWwindow* m_window;

    bool m_needRender = true;
    bool m_isInitialized = false;
};

} // namespace ciel
