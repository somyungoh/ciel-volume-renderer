#include "cielApp.h"

#include <cstring>
#include <iostream>
#include <stdexcept>

namespace ciel {

void CielApp::run()
{
    if (!m_isInitialized) {
        std::cerr << "Attempted to run Ciel App before initialized. Abort.\n";
        return;
    }

    mainLoop();

    cleanup();
}

void CielApp::init(int windowW, int windowH)
{
    initGLFW(windowW, windowH);
    glfwSetWindowUserPointer(m_window, this);

    m_renderer = std::make_unique<Renderer>();

    // render setting
    m_renderSetting.renderW = 400; // TODO: proportion to window size
    m_renderSetting.renderH = 300; // TODO: proportion to window size
    m_renderSetting.rayDt = 0.01;
    m_renderSetting.expK = 0.02;

    m_isInitialized = true;
}

void keyCallback(
    GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        CielApp* app = static_cast<CielApp*>(glfwGetWindowUserPointer(window));
        RenderSetting setting = app->getRenderSetting();

        if (key == GLFW_KEY_S) {
            setting.rayDt -= 0.0001;
            std::cout << "Change rayDt: " << setting.rayDt << std::endl;
        }
        if (key == GLFW_KEY_D) {
            setting.rayDt += 0.0001;
            std::cout << "Change rayDt: " << setting.rayDt << std::endl;
        }
        if (key == GLFW_KEY_W) {
            setting.expK -= 0.001;
            std::cout << "Change expK: " << setting.expK << std::endl;
        }
        if (key == GLFW_KEY_E) {
            setting.expK += 0.001;
            std::cout << "Change expK: " << setting.expK << std::endl;
        }

        app->setRenderSetting(setting);

        // Execute Render
        if (key == GLFW_KEY_SPACE) {
            app->setNeedRender(true);
        }
    }
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
}

void CielApp::mainLoop()
{

    while (!glfwWindowShouldClose(m_window)) {

        // Execute render if needed
        if (m_needRender) {
            m_renderer->Render(m_renderSetting);
            m_needRender = false;
        }
        std::vector<float> pixmap = m_renderer->getLastRender();

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
        glPixelZoom(fbuffer_w / m_renderSetting.renderW,
                    fbuffer_h / m_renderSetting.renderH);
        glDrawPixels(m_renderSetting.renderW,
                     m_renderSetting.renderH,
                     GL_RGBA,
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