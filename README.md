# Hello GPU Compute Shader

This is a simple example of an OpenGL compute shader using a shared shader buffer object (SSBO). Coincidentally, it simultaneously uses a texture based mechanism for data exchange because that's how some of the code worked that this example is based upon. I've left that code in place to contrast the texture approach with SSBOs. The SSBO, in my opinion, is much better suited for handling arbitrary data structures.

It doesn't do much except prove that I can pass data to and from a compute shader and do some simple calculations to modify the data in the shader. 

I specifically want to use the compute shader for performing non-graphical calculations so this is an example of using it independent of any graphics rendering pipeline, hence the output is all sent to stdout.

It uses code from several different resources that I found online. The most significant sample code and video references are noted in the code comments. My thanks to all who posted those references.

The project is in C++ using Visual Studio 2022. I eventually plan to experiment with adding compute shaders to an existing Java (JOGL) application, so this was just an interum step. I don't plan to do much more with this C++ code except post it on github as a working example for anyone else who might find it useful.

Enjoy

;-)
