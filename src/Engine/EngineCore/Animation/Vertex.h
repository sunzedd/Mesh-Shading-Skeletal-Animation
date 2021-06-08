#pragma once
#include "../Core/Alias.h"

namespace FQW {

struct Vertex
{
    vec3  position;
    vec3  normal;
    ivec4 boneIds = ivec4(0);
    vec4  boneWeights = vec4(0.0f);
};

}