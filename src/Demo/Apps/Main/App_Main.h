#pragma once
#include <Engine.h>

#include "../../Scripts/ModelScript.h"



namespace FQW::Main {

class App_Main : public Application
{
public:
    static const int WIDTH = 1280;
    static const int HEIGHT = 720;
    string MODEL_FILEPATH = s_SolutionDirectory + "res\\meshes\\working\\spider.fbx";

    Ref<CameraFPS>  _camera;
    Ref<Model>      _model;
    Ref<Animator>   _animator;

    Unique<ShaderPipeline> _shaderPipeline;

    vector<Ref<ScriptableEntity>> _scriptables;


public:
    App_Main();

    void Init();
    void SetupCamera();
    void SetupScene();
    virtual void SetupShader();

    virtual void Render() override;
    void Start() override;
    void Update(float deltaTime) override;
    void DrawUI() override;


#if LOG_ANIMATION_DATA
    void LogSkeletonHierarchy(const Bone& bone, std::string logOffset = "");
    void LogAnimationTransformations(Ref<Animation> animation);
#endif
};

}