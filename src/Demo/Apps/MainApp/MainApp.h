#pragma once
#include <CameraScript.h>
#include "ModelScript.h"

#include <EngineCore/Graphics/ClassicShaderPipeline.h>
#include <EngineCore/Core/Alias.h>
#include <EngineCore/Core/Application.h>
#include <EngineCore/Scene/CameraFPS.h>
#include <Animation/Animator.h>
#include <Animation/Model.h>
#include <Animation/ModelLoader.h>



namespace FQW::MainApp {

class MainApp : public Application
{
public:
    static const int WIDTH = 1280;
    static const int HEIGHT = 720;
    const std::string MODEL_FILEPATH = s_SolutionDirectory + "res\\meshes\\working\\spider.fbx";

    Ref<CameraFPS> _camera;

    Unique<ShaderPipeline> _shaderPipeline;
    Ref<IDrawable> _drawableModel;
    Ref<Model> _model;
    Ref<Animator> _animator;

    std::vector<Ref<ScriptableEntity>> _scriptables;


public:
    MainApp();

    void SetupCamera();
    virtual void SetupDrawable();

    void LogSkeletonHierarchy(const Bone& bone, std::string logOffset = "");

    void LogAnimationTransformations(Ref<Animation> animation);


    virtual void Render() override;
    void Start() override;
    void Update(float deltaTime) override;
    void DrawUI() override;
};

}