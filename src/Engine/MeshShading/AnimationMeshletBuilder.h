#pragma once
#include <set>
#include <map>
#include <algorithm>

#include "NaiveMeshletBuilder.h"


namespace FQW {

class AnimationMeshletBuilder final : public IMeshletBuilder
{
private:
    template <typename T>
    struct Triangle
    {
        T A;
        T B;
        T C;
    };

    using TriangleIndices = Triangle<uint32_t>;

public:
    AnimationMeshletBuilder(float eps = 0.01f)
    {
        SetEpsilon(eps);
    }

    vector<Meshlet> Build(const vector<Vertex>& vertexBuffer, const vector<uint32_t>& indexBuffer) override
    {
        FQW_TRACE("[AnimationMeshletBuilder] Building meshlets ...");

        if (m_OmegaGroup.size() > 0)
            m_OmegaGroup.clear();
        if (m_BettaGroups.size() > 0)
            m_BettaGroups.clear();
        if (m_AlphaGroups.size() > 0)
            m_AlphaGroups.clear();

        FindBettaAndOmegaTris(vertexBuffer, indexBuffer);
        FindAlphaTris(vertexBuffer);
        vector<Meshlet> resultMeshlets = ComposeMeshlets(vertexBuffer);

        FQW_TRACE("[AnimationMeshletBuilder] Alpha = {}, Betta = {}, Omega = {}",
                  m_MeshletCounter.Alpha, m_MeshletCounter.Betta, m_MeshletCounter.Omega);

        return resultMeshlets;
    }

    void SetEpsilon(float eps)
    {
        if (eps < 0.0001f)
            m_Eps = 0.0001f;
        else if (eps > 0.5f)
            m_Eps = 0.5f;
        else
            m_Eps = eps;
    }

private:
    void FindBettaAndOmegaTris(const vector<Vertex>& vertexBuffer, const vector<uint32_t>& indexBuffer)
    {
        for (size_t i = 0; i < indexBuffer.size(); i += 3)
        {
            uint32_t a = indexBuffer[i + 0];
            uint32_t b = indexBuffer[i + 1];
            uint32_t c = indexBuffer[i + 2];

            const Vertex& va = vertexBuffer[a];
            const Vertex& vb = vertexBuffer[b];
            const Vertex& vc = vertexBuffer[c];

            TriangleIndices t{ a, b, c };
            if (CompareBoneIDs(va.boneIDs, vb.boneIDs, vc.boneIDs) != 3)
            {
                m_OmegaGroup.push_back(t);
            }
            else
            {
                uint64_t hash = CalculateBettaGroupHash(va, vb, vc);

                auto bettaGroupIter = m_BettaGroups.find(hash);

                if (bettaGroupIter != m_BettaGroups.end())
                    (*bettaGroupIter).second.push_back(t);
                else
                    m_BettaGroups.insert({ hash, { t } });
            }
        }
    }

    uint32_t CompareBoneIDs(const ivec4& a, const ivec4& b, const ivec4& c)
    {
        uint32_t equalsCount = 0;

        std::set<int> sa = { a.x, a.y, a.z, a.w };
        std::set<int> sb = { b.x, b.y, b.z, b.w };
        std::set<int> sc = { c.x, c.y, c.z, c.w };

        if (sa == sb)
            equalsCount = equalsCount + 2;
        if (sa == sc)
            equalsCount++;

        return equalsCount;
    }

    uint64_t CalculateBettaGroupHash(const Vertex& a, const Vertex& b, const Vertex& c)
    {
        assert(CompareBoneIDs(a.boneIDs, b.boneIDs, c.boneIDs) == 3);

        vector<int> IDs = { a.boneIDs.x, a.boneIDs.y, a.boneIDs.z, a.boneIDs.w };
        std::sort(IDs.begin(), IDs.end());

        uint64_t hash = IDs[0];
        hash << 48;
        hash |= IDs[1]; hash << 32;
        hash |= IDs[2]; hash << 16;
        hash |= IDs[3]; hash << 0;

        return hash;
    }

    void FindAlphaTris(const vector<Vertex>& vertexBuffer)
    {
        for (auto& [bettaGroupHash, bettaGroupTris] : m_BettaGroups)
        {
            vector< std::list<TriangleIndices>::iterator > trisToEraseFromBetta;

            for (auto triIter = bettaGroupTris.begin(); triIter != bettaGroupTris.end(); triIter++)
            {
                if (TriangleAlphaTest(*triIter, vertexBuffer))
                {
                    auto alphaGroupIter = m_AlphaGroups.find(bettaGroupHash);

                    if (alphaGroupIter != m_AlphaGroups.end())
                        (*alphaGroupIter).second.push_back(*triIter);
                    else
                        m_AlphaGroups.insert({ bettaGroupHash, {*triIter} });

                    trisToEraseFromBetta.push_back(triIter);
                }
            }

            for (auto& triToErase : trisToEraseFromBetta)
                m_BettaGroups[bettaGroupHash].erase(triToErase);
        }

        EraseEmptyBettaGroups();
    }

    bool TriangleAlphaTest(const TriangleIndices& tIndices, const vector<Vertex>& vertexBuffer)
    {
        using BoneWeight = std::pair<int, float>; // int - boneID, float - it's weight

        const Vertex& a = vertexBuffer[tIndices.A];
        const Vertex& b = vertexBuffer[tIndices.B];
        const Vertex& c = vertexBuffer[tIndices.C];

        BoneWeight wa[4] = {
            { a.boneIDs.x, a.boneWeights.x },
            { a.boneIDs.y, a.boneWeights.y },
            { a.boneIDs.z, a.boneWeights.z },
            { a.boneIDs.w, a.boneWeights.w }
        };

        BoneWeight wb[4] = {
            { b.boneIDs.x, b.boneWeights.x },
            { b.boneIDs.y, b.boneWeights.y },
            { b.boneIDs.z, b.boneWeights.z },
            { b.boneIDs.w, b.boneWeights.w }
        };

        BoneWeight wc[4] = {
            { c.boneIDs.x, c.boneWeights.x },
            { c.boneIDs.y, c.boneWeights.y },
            { c.boneIDs.z, c.boneWeights.z },
            { c.boneIDs.w, c.boneWeights.w }
        };

        auto Comparator = [](BoneWeight bw0, BoneWeight bw1) -> bool { return bw0.first < bw1.first; };

        std::sort(wa, wa + 4, Comparator);
        std::sort(wb, wb + 4, Comparator);
        std::sort(wc, wc + 4, Comparator);

        bool testFailed = false;
        for (int i = 0; i < 4 && !testFailed; i++)
        {
            if (fabs(wa[0].second - wb[0].second) > m_Eps)
                testFailed = true;

            if (fabs(wa[0].second - wc[0].second) > m_Eps)
                testFailed = true;
        }

        return !testFailed;
    }

    void EraseEmptyBettaGroups()
    {
        vector < std::map<uint64_t, std::list<TriangleIndices>>::iterator > emptyBettaGroups;
        for (auto groupIter = m_BettaGroups.begin(); groupIter != m_BettaGroups.end(); groupIter++)
        {
            if (groupIter->second.empty())
                emptyBettaGroups.push_back(groupIter);
        }

        for (auto& groupToErase : emptyBettaGroups)
            m_BettaGroups.erase(groupToErase->first);
    }

    vector<Meshlet> ComposeMeshlets(const vector<Vertex>& vertexBuffer)
    {
        NaiveMeshletBuilder naiveMeshletBuilder;

        vector<uint32_t> omegaIndices = ConvertIndexBuffer(m_OmegaGroup);
        vector<Meshlet> omegaMeshlets = naiveMeshletBuilder.Build(vertexBuffer, omegaIndices);
        for (auto& meshlet : omegaMeshlets)
            meshlet.classID = Meshlet::Omega;
        m_MeshletCounter.Omega = omegaMeshlets.size();

        // TODO: 
        // Sort triangles to be close to each other.
        // take 1 triangle T_pivot, fetch its average coords (x, y, z)
        // loop on all triangles except T_pivot
        //      fetch average coords (x, y, z)
        //      calculate distance to T_pivot
        //      Sort by distance

        vector<Meshlet> bettaMeshlets;
        for (auto& bettaGroup : m_BettaGroups)
        {
            vector<uint32_t> groupIndices = ConvertIndexBuffer(bettaGroup.second);
            vector<Meshlet> meshlets = naiveMeshletBuilder.Build(vertexBuffer, groupIndices);
           
            bettaMeshlets.insert(bettaMeshlets.end(), meshlets.begin(), meshlets.end());
        }
        for (auto& meshlet : bettaMeshlets)
            meshlet.classID = Meshlet::Betta;
        m_MeshletCounter.Betta = bettaMeshlets.size();


        vector<Meshlet> alphaMeshlets;
        for (auto& alphaGroup : m_AlphaGroups)
        {
            vector<uint32_t> groupIndices = ConvertIndexBuffer(alphaGroup.second);
            vector<Meshlet> meshlets = naiveMeshletBuilder.Build(vertexBuffer, groupIndices);

            alphaMeshlets.insert(alphaMeshlets.end(), meshlets.begin(), meshlets.end());
        }
        for (auto& meshlet : alphaMeshlets)
            meshlet.classID = Meshlet::Alpha;
        m_MeshletCounter.Alpha = alphaMeshlets.size();

        vector<Meshlet> resultMeshlets = omegaMeshlets;
        resultMeshlets.insert(resultMeshlets.end(), bettaMeshlets.begin(), bettaMeshlets.end());
        resultMeshlets.insert(resultMeshlets.end(), alphaMeshlets.begin(), alphaMeshlets.end());

        return resultMeshlets;
    }

    static vector<uint32_t> ConvertIndexBuffer(const vector<TriangleIndices>& triangleIndices)
    {
        vector<uint32_t> result(triangleIndices.size() * 3);
        std::memcpy(result.data(), triangleIndices.data(), sizeof(uint32_t) * result.size());
        return result;
    }

    static vector<uint32_t> ConvertIndexBuffer(const std::list<TriangleIndices>& triangleIndices)
    {
        vector<uint32_t> result(triangleIndices.size() * 3);
        for (auto t : triangleIndices)
        {
            result.push_back(t.A);
            result.push_back(t.B);
            result.push_back(t.C);
        }
        return result;
    }

private:
    std::vector<TriangleIndices> m_OmegaGroup;
    std::map<uint64_t, std::list<TriangleIndices>> m_BettaGroups;
    std::map<uint64_t, std::vector<TriangleIndices>> m_AlphaGroups;

    float m_Eps;

    struct {
        size_t Alpha = 0;
        size_t Betta = 0;
        size_t Omega = 0;
    } m_MeshletCounter;
};

} // namespace FQW
