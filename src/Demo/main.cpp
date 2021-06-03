#if 1
#include "Apps/Main/App_Main.h"

int main()
{
    FQW::Main::App_Main app;
    app.Init();
    app.Run();
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