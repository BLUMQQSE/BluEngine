#pragma once
#include "Inventory.h"
namespace bm98
{
class CombatInventory :
    public Inventory
{
public:
    CombatInventory() {}
    virtual ~CombatInventory() {}

    virtual void init() override;
};

}