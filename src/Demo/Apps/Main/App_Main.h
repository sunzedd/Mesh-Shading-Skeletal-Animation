#pragma once
#include <Engine.h>

#include "../../Scripts/ModelScript.h"
#include "../../Scripts/ApplicationScript.h"



namespace FQW::Main {

class App_Main : public Application
{
public:
    static const int WIDTH = 1280;
    static const int HEIGHT = 720;
    string MODEL_FILEPATH = s_SolutionDirectory + "res\\meshes\\working\\spider.fbx";

    Unique<ShaderPipeline> _ShaderPipeline;
    Ref<Model>      _Model;
    Ref<Animator>   _Animator;


public:
    App_Main();

    void Init();
    void SetupScene();
    virtual void SetupShader();

    virtual void Render() override;
    void DrawUI() override;


#if LOG_ANIMATION_DATA
    void LogSkeletonHierarchy(const Bone& bone, std::string logOffset = "");
    void LogAnimationTransformations(Ref<Animation> animation);
#endif
};

}