#include "OpenGL_Utils.h"

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


namespace FQW {

std::string OpenGL_GetGraphicsDeviceInfo()
{
    std::string result = "GPU Info\n";

    glcheck( const char* vendorName = (char*)glGetString(GL_VENDOR) );
    glcheck( const char* rendererName = (char*)glGetString(GL_RENDERER) );

    return result + "\tVendor: " + vendorName + "\n\tRenderer: " + rendererName;
}


std::pair<int, int> OpenGL_GetVersion()
{
    int major, minor;
    glcheck( glGetIntegerv(GL_MAJOR_VERSION, &major) );
    glcheck( glGetIntegerv(GL_MINOR_VERSION, &minor) );

    return std::make_pair(major, minor);

}


bool OpenGL_CheckExtensionSupport(std::string extensionName)
{
    int extensionCount = 0;
    glcheck( glGetIntegerv(GL_NUM_EXTENSIONS, &extensionCount) );

    bool extensionSupported = false;

    for (int i = 0; i < extensionCount && !extensionSupported; i++)
    {
        glcheck(const char* supportedExtension = (char*)glGetStringi(GL_EXTENSIONS, i));
        if (supportedExtension == extensionName)
        {
            extensionSupported = true;
        }
    }

    return extensionSupported;
}

} // namespace FQW