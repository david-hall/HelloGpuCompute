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

    // See the "Shader binding index setting" section of
    // https://www.khronos.org/opengl/wiki/Interface_Block_(GLSL)#Shader_binding_index_setting
    // ssboIndex is arbitrary but it has to match the corresponding binding point of ioBuffer in the GLSL source
    // TODO: Look up GPU resources by name using glGetProgramResourceIndex(GLuint program, GLenum programInterface, const char* name);
    // See https://www.khronos.org/opengl/wiki/Program_Introspection
    // and https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetProgramResourceIndex.xhtml
    ssboIndex = 0;

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

        //std::cout << "GLSL:\n" << computeCode << std::endl;
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
    showShaderInfoLog(shaderId);

    // create program
    programId = glCreateProgram();
    glAttachShader(programId, shaderId);
    glLinkProgram(programId);
    showProgramInfoLog();

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
    setTextureValues(NULL);
    glBindImageTexture(0, ioTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);

    // create shared shader buffer object
    // see https://www.khronos.org/opengl/wiki/Shader_Storage_Buffer_Object#OpenGL_usage
    glGenBuffers(1, &ioBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ioBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssboIndex, ioBuffer);
}

ComputeShader::~ComputeShader() {
    glDeleteBuffers(1, &ioBuffer);
    glDeleteTextures(1, &ioTexture);
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

void ComputeShader::setTextureValues(float* values) {
    glTexImage2D(GL_TEXTURE_2D, /* level */ 0, GL_R32F, workSizeX, workSizeY, /*int border*/ 0, GL_RED, GL_FLOAT, /*const void *pixels */ values);
}

std::vector<float> ComputeShader::getTextureValues() {
    std::vector<float> textureValues(workVolume);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, textureValues.data());
    return textureValues;
}

void ComputeShader::setBufferValues(int* values) {
    glBufferData(GL_SHADER_STORAGE_BUFFER, workVolume * sizeof(values[0]), values, GL_DYNAMIC_READ);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssboIndex, ioBuffer);
}

std::vector<int> ComputeShader::getBufferValues() {
    std::vector<int> bufferValues(workVolume);
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, workVolume * sizeof(int), bufferValues.data());
    return bufferValues;
}

void ComputeShader::showShaderInfoLog(unsigned int shaderId) {
    int length;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
    if (length) {
        char* infoLog = new char[length];
        glGetShaderInfoLog(shaderId, length, &length, infoLog);
        std::cout.write(infoLog, length);
        std::cout << std::endl;
        delete[] infoLog;
    }
}

void ComputeShader::showProgramInfoLog() {
    int length;
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);
    if (length) {
        char* infoLog = new char[length];
        glGetProgramInfoLog(programId, length, &length, infoLog);
        std::cout.write(infoLog, length);
        std::cout << std::endl;
        delete[] infoLog;
    }
}

