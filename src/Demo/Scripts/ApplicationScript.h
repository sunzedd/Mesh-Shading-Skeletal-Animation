#pragma once
#include <EngineCore/Core/Application.h>


namespace FQW {

class ApplicationScript : public Script
{
public:
    Ref<Application> ThisApplication;


    void FirstSetup()
    {
        ThisApplication = std::static_pointer_cast<Application>(m_Entity);
    }


    void ApplicationScript::Update(float deltaTime)
    {
        if (Input::IsKeyPressed(GLFW_KEY_ESCAPE))
        {
            ThisApplication->Shutdown();
        }
    }
};

}