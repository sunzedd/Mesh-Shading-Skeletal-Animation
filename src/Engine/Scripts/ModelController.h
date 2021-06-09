#pragma once
#include "../EngineCore/Scene/Script.h"
#include "../EngineCore/Animation/Model.h"


namespace FQW {

class ModelController : public Script
{
protected:
    Ref<Model> This;

public:
    void FirstSetup() override
    {
        This = std::static_pointer_cast<Model>(m_Entity);
    }
};

}