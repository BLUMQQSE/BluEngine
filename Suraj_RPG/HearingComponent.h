#pragma once
#include "SensoryComponent.h"
namespace bm98
{
class HearingComponent :
    public SensoryComponent
{

public:
    HearingComponent();
    virtual ~HearingComponent();

    virtual void update() override;

};

}
