#include "OpenGL_Debug.h"

void OpenGL_ClearErrors()
{
    while (glGetError() != GL_NO_ERROR);
}


bool OpenGL_Log(const char* function, const char* file, int line)
{
    bool noErrorsOccured = true;
    while (GLenum error = glGetError())
    {
        FQW_ERROR("[OpenGL Error]: {}\n\tfile: {}\n\tfunction: {}\n\tline: {}", error, file, function, line);
        noErrorsOccured = false;
    }

    return noErrorsOccured;
}
