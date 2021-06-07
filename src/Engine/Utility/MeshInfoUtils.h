#pragma once
#include "../EngineCore/Animation/Model.h"

namespace FQW {

void LogSkeletonHierarchy(const Bone& bone, const string& logOffset)
{
    if (bone.name.empty()) {
        FQW_WARN("{} unnamed {}", logOffset, bone.name);
    }
    else {
        FQW_TRACE("{} {}", logOffset, bone.name);
    }

    for (const auto& child : bone.children) {
        LogSkeletonHierarchy(child, logOffset + "   ");
    }
}


void LogAnimationTransformations(Ref<Animation> animation) 
{
    for (const auto& transform : animation->boneTransforms) 
    {
        const string& name = transform.first;
        if (name.empty()) 
        {
            FQW_WARN("[LogAnimationTransformations]: transformation unnamed {}", name);
        }
    }
}

}