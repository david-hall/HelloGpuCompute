#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <iomanip>

#define DEBUG_ACTIVE true
#if DEBUG_ACTIVE
void GLAPIENTRY gl_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param);
#endif

#include "ComputeShader.h"

// adapted from https://www.glfw.org/documentation.html

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello GPU Compute", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwIconifyWindow(window);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    std::cout << "GL_VERSION: " << glGetString(GL_VERSION) << std::endl;
    //std::cout << "GL_SHADING_LANGUAGE_VERSION: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    //int mmba;
    //glGetIntegerv(GL_MIN_MAP_BUFFER_ALIGNMENT, &mmba);
    //std::cout << "GL_MIN_MAP_BUFFER_ALIGNMENT: " << mmba << std::endl;

    // init glew after context is current and before ComputeShader c'tor
    if (glewInit() != GLEW_OK) {
        std::cerr << "glewInit failed" << std::endl;
        glfwTerminate();
        return -2;
    }

#if DEBUG_ACTIVE
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(gl_message_callback, nullptr);
#endif

    // This doesn't seem to matter since I'm not rendering anything but
    // I'll disable any frame-per-second rendering limits anyway.
    glfwSwapInterval(0);

    // TODO: change values to use ints
    const unsigned int nValues = 10;
    float textureValues[nValues] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int bufferValues[nValues] = { 1, 1, 2, 3, 5, 8, 13, 21, 33, 54 };

    ComputeShader computeShader("res\\shader.comp", nValues, 1);
    computeShader.use();
    computeShader.setTextureValues(textureValues);
    computeShader.setBufferValues(bufferValues);

    //time_t prev = 0;
    //time_t now = 0;

    /* Loop until the user closes the window */
    unsigned int loops = 0;
    while (loops < 15 && !glfwWindowShouldClose(window)) {
        //if (prev != std::time(&now)) {
        //    prev = now;
            // only retrieve and display the values once per second
            // so we don't bottleneck the GPU's calculations
            loops++;
            std::cout << "texture: ";
            auto texData = computeShader.getTextureValues();
            for (auto d : texData) {
                std::cout << std::fixed << std::setw(10) << std:: setprecision(0) << std::setfill(' ')
                    << d << " ";
            }
            std::cout << std::endl;

            std::cout << "buffer:  ";
            auto bufData = computeShader.getBufferValues();
            for (auto d : bufData) {
                std::cout << std::fixed << std::setw(10) << std::setfill(' ')
                    << d << " ";
            }
            std::cout << std::endl;
            
            std::cout << std::endl;
        //}

        // update
        computeShader.use();
        computeShader.dispatch();
        computeShader.wait();

        // Since I don't care to render anything, I will comment out this rendering code
        // leaving just the GPU computations in the loop
        //if(false) {
        //    /* Render here */
        //    glClear(GL_COLOR_BUFFER_BIT);
        //    // https://youtu.be/OR4fNpBjmq8?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&t=1135
        //    glBegin(GL_TRIANGLES);
        //    glVertex2f(-0.5f, -0.5f);
        //    glVertex2f(0.0f, 0.5f);
        //    glVertex2f(0.5f, -0.5f);
        //    glEnd();
        //    /* Swap front and back buffers */
        //    glfwSwapBuffers(window);
        //}
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

#if DEBUG_ACTIVE
// more information at https://www.khronos.org/opengl/wiki/Debug_Output
void GLAPIENTRY gl_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param) {
    // parse source
    std::string source_name;
    switch (source) {
    case GL_DEBUG_SOURCE_API:
        source_name = "GL_DEBUG_SOURCE_API";
        break;

    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        source_name = "GL_DEBUG_SOURCE_WINDOW_SYSTEM";
        break;

    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        source_name = "GL_DEBUG_SOURCE_SHADER_COMPILER";
        break;

    case GL_DEBUG_SOURCE_THIRD_PARTY:
        source_name = "GL_DEBUG_SOURCE_THIRD_PARTY";
        break;

    case GL_DEBUG_SOURCE_APPLICATION:
        source_name = "GL_DEBUG_SOURCE_APPLICATION";
        break;

    case GL_DEBUG_SOURCE_OTHER:
        source_name = "GL_DEBUG_SOURCE_OTHER";
        break;

    default:
        source_name = "UNKNOWN";
        break;
    }

    // parse type
    std::string type_name;
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        type_name = "GL_DEBUG_TYPE_ERROR";
        break;

    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        type_name = "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
        break;

    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        type_name = "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
        break;

    case GL_DEBUG_TYPE_PORTABILITY:
        type_name = "GL_DEBUG_TYPE_PORTABILITY";
        break;

    case GL_DEBUG_TYPE_PERFORMANCE:
        type_name = "GL_DEBUG_TYPE_PERFORMANCE";
        break;

    case GL_DEBUG_TYPE_MARKER:
        type_name = "GL_DEBUG_TYPE_MARKER";
        break;

    case GL_DEBUG_TYPE_PUSH_GROUP:
        type_name = "GL_DEBUG_TYPE_PUSH_GROUP";
        break;

    case GL_DEBUG_TYPE_POP_GROUP:
        type_name = "GL_DEBUG_TYPE_POP_GROUP";
        break;

    case GL_DEBUG_TYPE_OTHER:
        type_name = "GL_DEBUG_TYPE_OTHER";
        break;

    default:
        type_name = "UNKNOWN";
        break;
    }

    // parse severity
    std::string severity_name;
    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        severity_name = "GL_DEBUG_SEVERITY_HIGH";
        break;

    case GL_DEBUG_SEVERITY_MEDIUM:
        severity_name = "GL_DEBUG_SEVERITY_MEDIUM";
        break;

    case GL_DEBUG_SEVERITY_LOW:
        severity_name = "GL_DEBUG_SEVERITY_LOW";
        break;

    case GL_DEBUG_SEVERITY_NOTIFICATION:
        severity_name = "GL_DEBUG_SEVERITY_NOTIFICATION";
        break;

    default:
        severity_name = "UNKNOWN";
        break;
    }

    // print message
    std::cerr << "GL CALLBACK:\n";
    std::cerr << "source: " << source_name << "\n";
    std::cerr << "type: " << type_name << "\n";
    std::cerr << "severity: " << severity_name << "\n";
    std::cerr << "message: " << message << "\n";
    std::cerr << std::endl;
}
#endif
