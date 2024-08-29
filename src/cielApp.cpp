
//
//  Main Ciel Application
//
//  Large amount of openGL logic has been copied and modified from
//  https://learnopengl.com/
//

#include "cielApp.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"

#ifdef __APPLE__
#include <OpenGL/gl3.h> // macOS openGL library
#endif

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
    initImGui();
    glfwSetWindowUserPointer(m_window, this);

    m_renderer = std::make_unique<Renderer>();

    // render setting
    m_renderSetting.renderW = windowW; // TODO: proportion to window size
    m_renderSetting.renderH = windowH; // TODO: proportion to window size
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
            setting.rayDt -= 0.00025;
            std::cout << "Change rayDt: " << setting.rayDt << std::endl;
        }
        if (key == GLFW_KEY_D) {
            setting.rayDt += 0.0005;
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

    // GL 3.2 + GLSL 150
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on Mac
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

void CielApp::initImGui()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(
        m_window, true); // Second param install_callback=true will install GLFW
                         // callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
}

constexpr char* vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;"
    "layout (location = 1) in vec3 aColor;"
    "layout (location = 2) in vec2 aTexCoord;"
    ""
    "out vec3 ourColor;"
    "out vec2 TexCoord;"
    "void main()"
    "{"
    "	gl_Position = vec4(aPos, 1.0);"
    "	ourColor = aColor;"
    "	TexCoord = vec2(aTexCoord.x, aTexCoord.y);"
    "}";
constexpr char* fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;"
    ""
    "in vec3 ourColor;"
    "in vec2 TexCoord;"
    ""
    "// texture samplers\n"
    "uniform sampler2D texture1;"
    ""
    "void main()"
    "{"
    "	FragColor = texture(texture1, TexCoord);"
    "}";

void CielApp::createGLShader(unsigned int& outShaderProgram)
{
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "[ciel][app] Vertex shader compilation failed!\n"
                  << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "[ciel][app] Fragment shader compilation failed!\n"
                  << infoLog << std::endl;
    }
    // link shaders
    outShaderProgram = glCreateProgram();
    glAttachShader(outShaderProgram, vertexShader);
    glAttachShader(outShaderProgram, fragmentShader);
    glLinkProgram(outShaderProgram);
    // check for linking errors
    glGetProgramiv(outShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(outShaderProgram, 512, NULL, infoLog);
        std::cout << "[ciel][app] Failed to link shader program!\n"
                  << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

constexpr float vertices[] = {
    // positions        // colors         // texture coords
    1.0f,  1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
    1.0f,  -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
    -1.0f, 1.0,   0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
};
constexpr unsigned int indices[] = {
    0,
    1,
    3, // first triangle
    1,
    2,
    3 // second triangle
};

void CielApp::createGLQuad(void*         textureData,
                           unsigned int& outVBO,
                           unsigned int& outVAO,
                           unsigned int& outEBO,
                           unsigned int& outTextureID)
{
    glGenVertexArrays(1, &outVAO);
    glGenBuffers(1, &outVBO);
    glGenBuffers(1, &outEBO);

    glBindVertexArray(outVAO);

    glBindBuffer(GL_ARRAY_BUFFER, outVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, outEBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          8 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          8 * sizeof(float),
                          (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenTextures(1, &outTextureID);
    glBindTexture(GL_TEXTURE_2D, outTextureID);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S,
                    GL_REPEAT); // set texture wrapping to GL_REPEAT (default
                                // wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (textureData) {
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGBA8,
                     m_renderSetting.renderW,
                     m_renderSetting.renderH,
                     0,
                     GL_RGBA,
                     GL_FLOAT,
                     textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "[ciel][app] Failed to load texture" << std::endl;
    }
};

void CielApp::mainLoop()
{
    // Execute render if needed
    if (m_needRender) {
        m_renderer->Render(m_renderSetting);
        m_needRender = false;
    }
    std::vector<float> pixmap = m_renderer->getLastRender();

    // openGL stuff for displaying the render
    unsigned int shaderProgram;
    createGLShader(shaderProgram);
    unsigned int VBO, VAO, EBO;
    unsigned int textureID;
    createGLQuad(pixmap.data(), VBO, VAO, EBO, textureID);

    // tell opengl for each sampler to which texture unit it belongs to (only
    // has to be done once)
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);

    // Render Routine
    while (!glfwWindowShouldClose(m_window)) {
        // input
        // processInput(window);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow(); // Show demo window! :)

        // Rendering
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // render container
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse
        // moved etc.)
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
}

void CielApp::cleanup()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

} // namespace ciel