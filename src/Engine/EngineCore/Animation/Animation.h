#pragma once
#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Core/Alias.h"


namespace FQW {

// Структура, содержащая дерево костей (скелет)
struct Bone
{
    int id = 0;         // индекс позиции кости в финальном массиве, который будет отправлен в шейдер
    string name;
    mat4 offset = mat4(1.0f);
    vector<Bone> children = {};
};


// Структура, представляющая собой трек анимации
struct BoneTransformTrack
{
    vector<float> positionTimestamps = {};
    vector<float> rotationTimestamps = {};
    vector<float> scaleTimestamps = {};

    vector<vec3> positions = {};
    vector<quat> rotations = {};
    vector<vec3> scales = {};
};


struct Animation
{
    float duration = 0.0f;
    float ticksPerSecond = 1.0f;

    // Ключ - имя кости, значение - набор трансформаций кости в ключевых кадрах
    std::unordered_map<std::string, BoneTransformTrack> boneTransforms = {};

    mat4 globalInverseTransform;
};


} // namespace FQW