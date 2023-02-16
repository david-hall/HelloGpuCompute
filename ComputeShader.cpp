#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ComputeShader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

// adapted from https ://medium.com/@daniel.coady/compute-shaders-in-opengl-4-3-d1c741998c03

ComputeShader::ComputeShader(const char* path, unsigned int x, unsigned int y) {
    work_size.x = x;
    work_size.y = y;

    // read in shader code
    std::string compute_code;
    std::ifstream file;

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        file.open(path);
        std::stringstream file_stream;
        file_stream << file.rdbuf();
        file.close();

        compute_code = file_stream.str();

        std::cout << "GLSL:\n" << compute_code << std::endl;
    }
    catch (std::ifstream::failure e) {
        std::cerr << "failed to read compute shader file" << std::endl;
        return;
    }

    const char* c_shader_code = compute_code.c_str();

    // compile shader
    unsigned int shader;

    shader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(shader, 1, &c_shader_code, NULL);
    glCompileShader(shader);

    // create program
    id = glCreateProgram();
    glAttachShader(id, shader);
    glLinkProgram(id);

    // cleanup
    glDeleteShader(shader);

    // create input/output textures
    glGenTextures(1, &out_tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, out_tex);

    // turns out we need this. huh.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // create empty texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, work_size.x, work_size.y, 0, GL_RED, GL_FLOAT, NULL);
    glBindImageTexture(0, out_tex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
}

ComputeShader::~ComputeShader() {
    glDeleteProgram(id);
}

void ComputeShader::use() {
    glUseProgram(id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, out_tex);
}

void ComputeShader::dispatch() {
    // just keep it simple, 2d work group
    glDispatchCompute(work_size.x, work_size.y, 1);
}

void ComputeShader::wait() {
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
}

void ComputeShader::set_values(float* values) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, work_size.x, work_size.y, 0, GL_RED, GL_FLOAT, values);
}

std::vector<float> ComputeShader::get_values() {
    unsigned int collection_size = work_size.x * work_size.y;
    std::vector<float> compute_data(collection_size);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, compute_data.data());

    return compute_data;
}
