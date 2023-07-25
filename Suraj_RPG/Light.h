#pragma once
#include "Component.h"
#include "IRenderable.h"

namespace bm98
{
class Light :
	public Component
{
public:

    Light();
    virtual ~Light();

    virtual void init() override;


    virtual void set_state(bool on) { this->on = on; }
    virtual void toggle() { on = !on; }

    const FloatConvex& get_bounds() { return bounds; }

protected:
    FloatConvex bounds;
    bool on = true;

};
}
