#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "ComputeShader.h"

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

    // init glew after context is current and before ComputeShader c'tor
    if (glewInit() != GLEW_OK) {
        std::cerr << "glewInit failed" << std::endl;
        glfwTerminate();
        return -2;
    }

    // This doesn't seem to matter since I'm not rendering anything but
    // I'll disable any frame-per-second rendering limits anyway.
    glfwSwapInterval(0);    

    // TODO: change values to use ints
    const unsigned int nValues = 10;
    float values[nValues] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    ComputeShader computeShader("res\\shader.comp", nValues, 1);
    computeShader.use();
    computeShader.set_values(values);

    time_t prev = 0;
    time_t now;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        if (prev != std::time(&now)) {
            prev = now;
            // only retrieve and display the values once per second
            // so we don't bottleneck the GPU's calculations
            auto data = computeShader.get_values();
            for (auto d : data) {
                std::cout << d << " ";
            }
            std::cout << std::endl;
        }

        // update
        computeShader.use();
        computeShader.dispatch();
        computeShader.wait();


        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // https://youtu.be/OR4fNpBjmq8?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&t=1135
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f( 0.0f,  0.5f);
        glVertex2f( 0.5f, -0.5f);
        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
