#pragma once

namespace FQW {

class IUpdatable
{
public:
    virtual void Update(float deltaTime) = 0;
};

} // namespace FQW