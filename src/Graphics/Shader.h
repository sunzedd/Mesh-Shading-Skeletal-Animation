#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Core/Logger.h"


namespace FQW {

class Shader
{
public:
    Shader(std::string vertexPath, std::string fragmentPath);

    unsigned int GetId() const { return m_Id; }
    unsigned int GetUniformLocation(const std::string& name) const;
    unsigned int GetAttributeLocation(const std::string& name) const;

    void Use();
    
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetMatrix4fv(const std::string& name, const glm::mat4& value) const;


private:
    unsigned int m_Id;
};

} // namespace FQW