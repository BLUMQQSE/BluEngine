#include "pch.h"
#include "CombatInventory.h"

namespace bm98
{

void CombatInventory::init()
{
	Inventory::init();
	inventory_type = InventoryNS::Type::COMBAT;
}

}