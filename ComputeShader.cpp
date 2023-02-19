#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ComputeShader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

// adapted from https://medium.com/@daniel.coady/compute-shaders-in-opengl-4-3-d1c741998c03

ComputeShader::ComputeShader(const char* path, unsigned int x, unsigned int y) {
    workSizeX = x;
    workSizeY = y;
    workSizeZ = 1;
    workVolume = workSizeX * workSizeY * workSizeZ;

    // read in shader code
    std::string computeCode;
    std::ifstream file;

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        file.open(path);
        std::stringstream fileStream;
        fileStream << file.rdbuf();
        file.close();

        computeCode = fileStream.str();

        std::cout << "GLSL:\n" << computeCode << std::endl;
    }
    catch (std::ifstream::failure e) {
        std::cerr << "failed to read compute shader file" << std::endl;
        return;
    }

    // compile shader
    unsigned int shaderId = glCreateShader(GL_COMPUTE_SHADER);
    const char* shaderSourceCode = computeCode.c_str();
    glShaderSource(shaderId, 1, &shaderSourceCode, NULL);
    glCompileShader(shaderId);

    // create program
    programId = glCreateProgram();
    glAttachShader(programId, shaderId);
    glLinkProgram(programId);

    // cleanup
    glDeleteShader(shaderId);

    // create input/output textures
    glGenTextures(1, &ioTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ioTexture);

    // turns out we need this. huh.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // create empty texture
    setValues(NULL);
    glBindImageTexture(0, ioTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
}

ComputeShader::~ComputeShader() {
    glDeleteProgram(programId);
}

void ComputeShader::use() {
    glUseProgram(programId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ioTexture);
}

void ComputeShader::dispatch() {
    // just keep it simple, 2d work group
    glDispatchCompute(workSizeX, workSizeY, 1); // ignore workSizeZ even though it should equal 1
}

void ComputeShader::wait() {
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
}

void ComputeShader::setValues(float* values) {
    glTexImage2D(GL_TEXTURE_2D, /* level */ 0, GL_R32F, workSizeX, workSizeY, /*int border*/ 0, GL_RED, GL_FLOAT, /*const void *pixels */ values);
}

std::vector<float> ComputeShader::getValues() {
    std::vector<float> shaderData(workVolume);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, shaderData.data());
    return shaderData;
}
