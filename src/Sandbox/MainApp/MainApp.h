#pragma once
#include "../CameraScript.h"
#include "ModelScript.h"

#include "../../Core/Alias.h"
#include "../../Core/Application.h"
#include "../../Scene/CameraFPS.h"
#include "../../Animation/Animator.h"
#include "../../Animation/Model.h"
#include "../../Animation/ModelLoader.h"



namespace FQW {

class MainApp : public Application
{
public:
    static const int WIDTH = 1280;
    static const int HEIGHT = 720;
    //const std::string MODEL_FILEPATH = "res/meshes/astroBoy_walk_Maya.dae";
    const std::string MODEL_FILEPATH = "res/meshes/man.dae";


    Unique<Shader> _shader;
    Ref<CameraFPS> _camera;

    Ref<IDrawable> _drawableModel;
    Ref<Model> _model;
    Ref<Animator> _animator;

    std::vector<Ref<ScriptableEntity>> _scriptables;


public:
    MainApp();

    void SetupCamera();
    void SetupDrawable();

    void LogSkeletonHierarchy(const Bone& bone, std::string logOffset = "");

    void LogAnimationTransformations(Ref<Animation> animation);


    void Render() override;
    void Start() override;
    void Update(float deltaTime) override;
};

}