#pragma once
#include "../EngineCore/Animation/Mesh.h"
#include "../EngineCore/Animation/Vertex.h"

#include "Meshlet.h"


namespace FQW {

class IMeshletBuilder
{
public:
    virtual vector<Meshlet> Build(const vector<Vertex>& vertexBuffer, const vector<uint32_t>& indexBuffer) = 0;
};

}