//
// Created by Rhys on 6/17/2023.
//

#include "client.h"

void debugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                  const GLchar *message, const void *userParam)
{
    // Print, log, whatever based on the enums and message
    if (severity >= GL_DEBUG_SEVERITY_HIGH)
        std::cout << std::hex << type << " : " << severity << " : " << message << std::endl;
}