#pragma once
#include <vector>

class ComputeShader {
public: 
    ComputeShader(const char * path, unsigned int x, unsigned int y);

    ~ComputeShader();

    void use();

    void dispatch();

    void wait();

    void set_values(float* values);

    std::vector<float> get_values();

private:
    unsigned int id;
    unsigned int out_tex;

    struct uvec2 {
        unsigned int x;
        unsigned int y;
    };

    uvec2 work_size;
};
