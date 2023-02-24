#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <iomanip>
#include "glDebug.h"
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
