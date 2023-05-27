#pragma once
#include "../data_assets/DataAssets.h"
#include "../Component.h"
namespace bm98
{

class Inventory :
    public Component
{
public:
    struct InventoryItem
    {
        InventoryItem()
        {
            item = nullptr;
            current_capacity = 0;
        }

        ItemData* item;
        int current_capacity;
    };

public:

    Inventory() {}
    virtual ~Inventory() {}

    virtual void init() override;

    // Inherited via IData
    virtual Json::Value serialize_json() override;
    virtual void unserialize_json(Json::Value obj) override;

    inline int get_size()
    {
        return max_size;
    }

    inline ItemData* get_item(int index)
    {
        return content[index].item;
    }

    inline int get_capacity(int index)
    {
        return content.at(index).current_capacity;
    }

    bool check_compatability(int index, ItemData* data);
    /// <summary>
    /// Returns the first empty index in the inventory. If the inventory is full, this function
    /// will return -1.
    /// </summary>
    int get_first_available_index();
    /// <summary>
    /// Returns all available indexes.
    /// </summary>
    std::vector<int> get_all_available_indexes();
    /// <summary>
    /// Returns the first empty index in the inventory, tho will find first match if available.
    /// If the inventory is full, this function will return -1.
    /// </summary>
    /// <returns></returns>
    int get_first_available_include_match(ItemData* item);
    /// <summary>
    /// Adds item at index and increases current_capacity by count.
    /// </summary>
    /// <returns>Returns 0 if all items get added. Returns remainder of any numbers
    /// unable to be added.</returns>
    int add_item(int index, ItemData* item, int count = 1);
    ItemData* remove_item(int index, int count = 1);

    virtual std::vector<Editor::SerializedVar> get_editor_values() override;

protected:
    std::vector<InventoryItem> content;
    int max_size;
    InventoryNS::Type inventory_type = InventoryNS::Type::GENERAL;

};

}