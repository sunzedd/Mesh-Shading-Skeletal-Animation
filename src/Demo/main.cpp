#if 1
#include "Apps/Main/App_Main.h"

int main()
{
    auto app = FQW::CreateRef<FQW::Main::App_Main>();
    app->Init();
    app->Run();
        
    //    app ->Init();
    //app->Run();
}

#else
#include "Apps/TestMeshShaders/App_TestMeshShaders.h"

int main()
{
    FQW::TestMeshShaders::App_TestMeshShader app;
    app.Init();
    app.Run();
}
#endif