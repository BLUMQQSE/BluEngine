#pragma once
#include "SensoryComponent.h"
namespace bm98
{
class VisualComponent :
    public SensoryComponent
{
public:

    VisualComponent();
    virtual ~VisualComponent();

    virtual void update() override;


};
}

