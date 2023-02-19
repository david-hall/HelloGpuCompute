#pragma once
#include <vector>

class ComputeShader {
public: 
    ComputeShader(const char* path, unsigned int x, unsigned int y);

    ~ComputeShader();

    void use();

    void dispatch();

    void wait();

    void setValues(float* values);

    std::vector<float> getValues();

private:
    unsigned int workSizeX;
    unsigned int workSizeY;
    unsigned int workSizeZ;
    unsigned int workVolume; // x * y * z

    unsigned int programId;
    unsigned int ioTexture;
};
