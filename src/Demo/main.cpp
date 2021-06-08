#include "Apps/MeshShaderDemo/App_MeshShaderDemo.h"

int main()
{
    auto app = FQW::CreateRef<FQW::MeshShaderDemo::App_MeshShaderDemo>();
    app->Init();
    app->Run();
}
