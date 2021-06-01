#if 0
#include "Sandbox/TestAnimation/TestAnimationApplication.h"

int main()
{
    FQW::TestAnimationApplication app;
    app.Run();
}
#elseif 0

#include "Sandbox/MainApp/MainApp.h"

int main()
{
    FQW::MainApp app;
    app.Run();
}

#else
#include "Apps/MeshShaderTest/MeshShaderTestApp.h"

int main()
{
    FQW::MeshShaderTest::MeshShaderTestApp app;
    app.Run();
}

#endif