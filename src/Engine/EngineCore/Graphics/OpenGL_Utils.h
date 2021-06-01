#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Core/Logger.h"


#define FQW_ASSERT(x) if (!(x)) __debugbreak();

#define glcheck(x) OpenGL_ClearErrors();\
    x;\
    FQW_ASSERT(OpenGL_Log(#x, __FILE__, __LINE__))


void OpenGL_ClearErrors();
bool OpenGL_Log(const char* function, const char* file, int line);


namespace FQW {

std::string OpenGL_GetGraphicsDeviceInfo();
std::pair<int, int> OpenGL_GetVersion();

bool OpenGL_CheckExtensionSupport(std::string extensionName);

}