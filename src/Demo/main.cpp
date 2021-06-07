#if 0
#include "Apps/Main/App_Main.h"

int main()
{
    auto app = FQW::CreateRef<FQW::Main::App_Main>();
    app->Init();
    app->Run();
}

#elif 1
#include "Apps/TestMeshShaders/App_TestMeshShaders.h"

int main()
{
    auto app = FQW::CreateRef<FQW::TestMeshShaders::App_TestMeshShader>();
    app->Init();
    app->Run();
}
#else
#include "Apps/TestMeshShaders2/App_TestMeshShaders2.h"

int main()
{
    auto app = FQW::CreateRef<FQW::TestMeshShaders2::App_TestMeshShader>();
    app->Init();
    app->Run();
}
#endif