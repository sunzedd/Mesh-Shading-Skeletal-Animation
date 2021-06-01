#pragma once
#include <CameraScript.h>
#include "ModelScript.h"

#include <EngineCore/Core/Alias.h>
#include <EngineCore/Core/Application.h>
#include <EngineCore/Scene/CameraFPS.h>
#include <Animation/Animator.h>
#include <Animation/Model.h>
#include <Animation/ModelLoader.h>

#include <MeshShading/NaiveMeshletBuilder.h>

#define TURING_PIPELINE 1


namespace FQW::MeshShaderTest {

class MeshShaderTestApp : public Application
{
public:
    static const int WIDTH = 1280;
    static const int HEIGHT = 720;
    const std::string MODEL_FILEPATH = s_SolutionDirectory + "res\\meshes\\working\\spider.fbx";


    Unique<Shader> _defaultPipelineShader;
    Unique<Shader> _turingPipelineShader;


    Ref<CameraFPS> _camera;

    Ref<IDrawable> _drawableModel;
    
    std::vector<MeshShaders::Meshlet> _modelMeshlets;
    Ref<Model> _model;

    Ref<Animator> _animator;

    std::vector<Ref<ScriptableEntity>> _scriptables;


public:
    MeshShaderTestApp();

    void SetupCamera();
    void SetupDrawable();

    void Render() override;
    void Start() override;
    void Update(float deltaTime) override;
    void DrawUI() override;

    bool _UseTuringPipeline = false;
    bool _Wireframe = false;
};

}