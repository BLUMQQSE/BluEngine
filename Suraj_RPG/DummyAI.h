#pragma once
#include "Component.h"
#include "globals.h"
#include "core/UITagSystem.h"

namespace bm98
{

class DummyAI :
    public Component
{

public:
    DummyAI();
    virtual ~DummyAI();

    virtual void init() override;

    virtual void update() override;

private:
    std::shared_ptr<TextUITag> name_tag;
    Vector2f name_tag_pos;
};

}
