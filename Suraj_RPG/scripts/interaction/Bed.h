#pragma once
#include "IInteractable.h"

namespace bm98
{

class Bed :
    public IInteractable
{
public:
    Bed() {}
    virtual ~Bed() {}

    virtual void init() override;

private:

    virtual void initiate_interaction(std::shared_ptr<Interactor> interactor) override;
    virtual void handle_interaction() override;
    virtual void exit_interaction() override;

    virtual Json::Value serialize_json() override;
    virtual void unserialize_json(Json::Value obj) override;

    GameObject* owner;
    float original_time_scale = INFINITY;

};

}