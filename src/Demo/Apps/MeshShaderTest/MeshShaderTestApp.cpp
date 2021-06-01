#include "MeshShaderTestApp.h"

namespace FQW::MeshShaderTest {

MeshShaderTestApp::MeshShaderTestApp()
    : Application(WIDTH, HEIGHT, "Test Animation")
{
    glEnable(GL_CULL_FACE);
    SetupDrawable();
    SetupCamera();
}


void MeshShaderTestApp::SetupCamera()
{
    _camera = CreateRef<CameraFPS>(glm::vec3(0, 0, 20));
    _camera->SetProjectionParameters((float)WIDTH / (float)HEIGHT, 60.0f);
    auto cameraScript = CreateRef<CameraScript>();
    Script::Link(_camera, cameraScript);
    _scriptables.push_back(std::static_pointer_cast<ScriptableEntity>(_camera));
}


void MeshShaderTestApp::SetupDrawable()
{
    // Loading assets
    _turingPipelineShader = CreateUnique<Shader>(
        GL_MESH_SHADER_NV,
        s_SolutionDirectory + "src\\Demo\\Apps\\MeshShaderTest\\res\\basic_mesh.mesh.glsl",
        s_SolutionDirectory + "src\\Demo\\Apps\\MeshShaderTest\\res\\basic_mesh.frag.glsl"
        );

    _defaultPipelineShader = CreateUnique<Shader>(
        GL_VERTEX_SHADER,
        s_SolutionDirectory + "res\\shaders\\animation\\shader.vs",
        s_SolutionDirectory + "res\\shaders\\animation\\shader.fs"
        );

    _model = ModelLoader::LoadModel(MODEL_FILEPATH);

    FQW_TRACE("Loaded animated model from {}", MODEL_FILEPATH);
    FQW_INFO("[NaiveMeshletBuilder] Building the meshlets ...");
    MeshShaders::NaiveMeshletBuilder builder;
    builder.BuildForMultipleMeshes(_model->GetMeshes());
    _modelMeshlets = builder.GetMeshlets();
    FQW_INFO("[NaiveMeshletBuilder] built {} meshlets", _modelMeshlets.size());
    _drawableModel = _model;
}


void MeshShaderTestApp::Render()
{
    if (_UseTuringPipeline) {
        _drawableModel->DrawWithMeshShader(*_turingPipelineShader, *_camera);
    } else {
        _drawableModel->Draw(*_defaultPipelineShader, *_camera);
    }
}


void MeshShaderTestApp::Start()
{
}

void MeshShaderTestApp::Update(float deltaTime)
{
    if (Input::IsKeyPressed(GLFW_KEY_ESCAPE)) Shutdown();
}

void MeshShaderTestApp::DrawUI()
{
    ImGui::Begin(u8"Производительность");
    ImGui::Text(u8"Время композиции кадра: %.3f милисекунд", m_DeltaTime);
    ImGui::Text(u8"Частота: %.1f кадров в секунду", 1000.0f / m_DeltaTime);

    if (ImGui::Checkbox(u8"Использовать сеточный шейдер", &_UseTuringPipeline)) 
    { }

    if (ImGui::Checkbox(u8"Каркас", &_Wireframe)) {
        if (_Wireframe) { glPolygonMode(GL_FRONT, GL_LINE); }
        else { glPolygonMode(GL_FRONT, GL_FILL); }
    }


    ImGui::SetWindowFontScale(1);
    ImGui::End();
}

} // namespace FQW