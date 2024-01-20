#include "cielApp.h"
#include "renderer.h"

#include <chrono>
#include <cstring>
#include <iostream>
#include <stdexcept>

namespace ciel {

CielApp::CielApp()
: m_renderer(std::make_unique<Renderer>()){};
CielApp::~CielApp() = default;

void CielApp::run()
{
    mainLoop();
    cleanup();
}

void keyCallback(
    GLFWwindow *window, int key, int scancode, int action, int mods)
{
    // if (key == GLFW_KEY_S && action == GLFW_PRESS)
    //     saveJpeg(renderSetting.render_w, renderSetting.render_h);
}

void CielApp::initGLFW(int width, int height)
{
    if (!glfwInit()) {
        printf("failed to initialze GLFW\n");
        return;
    }

    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    // create window
    m_window = glfwCreateWindow(
        width, height, "Ciel Renderer", nullptr, nullptr);

    glfwSetKeyCallback(m_window, keyCallback);
    glfwMakeContextCurrent(m_window);

    if (!m_window) {
        printf("Failed to initialize GLFW window");
        return;
    }
    else
        printf("Successfully initialized GLFW window");
}

void CielApp::mainLoop()
{
    // render setting
    RenderSetting renderSetting;
    renderSetting.render_w = 800;
    renderSetting.render_w = 600;

    // render!
    m_renderer->render(renderSetting);
    std::vector<float> pixmap = m_renderer->getLastRender();

    while (!glfwWindowShouldClose(m_window)) {
        // display
        int fbuffer_w, fbuffer_h;
        int window_w, window_h;
        glfwGetFramebufferSize(m_window, &fbuffer_w, &fbuffer_h);
        glfwGetWindowSize(m_window, &window_w, &window_h);

        glViewport(0, 0, fbuffer_w, fbuffer_h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, window_w, 0, window_h, 0, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glClear(GL_COLOR_BUFFER_BIT);
        glRasterPos2i(0, 0);
        // because (window size != framebuffer) size can happen
        glPixelZoom(fbuffer_w / renderSetting.render_w,
                    fbuffer_h / renderSetting.render_h);
        glDrawPixels(renderSetting.render_w,
                     renderSetting.render_h,
                     GL_RGB,
                     GL_FLOAT,
                     pixmap.data());

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void CielApp::cleanup()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

} // namespace ciel