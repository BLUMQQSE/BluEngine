#pragma once
#include "Component.h"
#include "AgentComponent.h"
namespace bm98
{
/// <summary>
/// Class which acts as an interface for all sensory components (visual, audio, etc).
/// </summary>
class SensoryComponent :
    public Component
{

public:
    SensoryComponent();
    virtual ~SensoryComponent();

    virtual void update() = 0;

private:
    float sensory_range;

};

}