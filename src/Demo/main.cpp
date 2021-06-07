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
#include <Engine.h>

int main()
{
    float vec[3];

    size_t vec3_size = sizeof(glm::vec3);
    size_t float3_size = sizeof(vec);

}
#endif