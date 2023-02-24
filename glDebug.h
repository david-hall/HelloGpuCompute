#pragma once

#define DEBUG_ACTIVE true
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
