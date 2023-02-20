#pragma once
#include <vector>

class ComputeShader {
public: 
    ComputeShader(const char* path, unsigned int x, unsigned int y);

    ~ComputeShader();

    void use();

    void dispatch();

    void wait();

    void setTextureValues(float* values);

    std::vector<float> getTextureValues();

    void setBufferValues(int* values);

    std::vector<int> getBufferValues();

private:
    void showShaderInfoLog(unsigned int shaderId);
    void showProgramInfoLog();

    unsigned int workSizeX;
    unsigned int workSizeY;
    unsigned int workSizeZ;
    unsigned int workVolume; // x * y * z

    unsigned int programId;
    unsigned int ioTexture;
    unsigned int ioBuffer;
    int ssboIndex;
};
