#pragma once
#include <EngineCore/Core/Alias.h>
#include <EngineCore/Graphics/OpenGL_Utils.h>

namespace FQW::MeshShaderDemo {

// https://www.lighthouse3d.com/tutorials/opengl-timer-query/
// Using double-buffering strategy for GPU time measurement
class GPUPerformanceProfiler 
{
public:
    GPUPerformanceProfiler()
    {
        glcheck(glGenQueries(2, m_QueryId));
        // dummy query to prevent OpenGL errors from popping out
        glcheck(glQueryCounter(m_QueryId[m_QueryFront], GL_TIMESTAMP));
    }

    // Nanoseconds
    uint64_t GetFrameTime_ns() const { return m_Time; }
    double GetFrameTime_ms() const { return m_Time / 1000000.0f; }

    void FrameBegin()
    {
        glcheck(glBeginQuery(GL_TIME_ELAPSED, m_QueryId[m_QueryBack]));
    }

    void FrameEnd()
    {
        glcheck(glEndQuery(GL_TIME_ELAPSED));
        glcheck(glGetQueryObjectui64v(m_QueryId[m_QueryBack], GL_QUERY_RESULT, &m_Time));
    }

private:
    uint32_t m_QueryId[2];
    uint32_t m_QueryBack = 0;
    uint32_t m_QueryFront = 1;
    GLuint64 m_Time = 0;
};

}