#pragma once
#include "IInteractable.h"
namespace bm98
{

class Chest :
    public IInteractable
{
public:
    Chest() {}
    virtual ~Chest() {}

    virtual void init() override;


protected:

    virtual void initiate_interaction(std::shared_ptr<Interactor> interactor) override;
    virtual void handle_interaction() override;
    virtual void exit_interaction() override;

    virtual Json::Value serialize_json() override;
    virtual void unserialize_json(Json::Value obj) override;



};

}