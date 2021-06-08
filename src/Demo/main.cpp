#if 0
#include "Apps/TestMeshShadersFullVertex/App_TestMeshShadersFullVertex.h"

int main()
{
    auto app = FQW::CreateRef<FQW::TestMeshShaders::App_TestMeshShaderFullVertex>();
    app->Init();
    app->Run();
}
#else
#include "Apps/MeshShaderDemo/App_MeshShaderDemo.h"

int main()
{
    auto app = FQW::CreateRef<FQW::MeshShaderDemo::App_MeshShaderDemo>();
    app->Init();
    app->Run();
}
#endif