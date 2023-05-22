#pragma once
#include "../Component.h"
#include "Inventory.h"
#include "../gui/Gui.h"
namespace bm98
{

class InventoryWindow;

class InventoryGUIController :
    public Component
{
public:
    InventoryGUIController();
    virtual ~InventoryGUIController();
    struct Hand
    {
        Inventory::InventoryItem item;
        Inventory* inventory;
        InventoryWindow* window;
        int index = 0;
        FloatConvex image;
        Vector2f image_size;
        Sorting::Layer layer = Sorting::Layer::UI;
        char z_order = 100;
        bool render = false;

        void init(Vector2f pos, Vector2f size)
        {
            image = FloatConvex::Polygon(Vector2f(pos.x, pos.y),
                                         {
                                             Vector2f(0, 0),
                                             Vector2f(size.x, 0),
                                             Vector2f(size.x, size.y),
                                             Vector2f(0, size.y)
                                         }
            );
            image_size = Vector2f(
                (image.getPoint(1).x - image.getPoint(0).x) * .5f,
                (image.getPoint(2).y - image.getPoint(1).y) * .5f
            );
            image.setFillColor(sf::Color::White);
            //render = true;

            clear();
        }

        void clear()
        {
            item = Inventory::InventoryItem();
            inventory = nullptr;
            window = nullptr;
            index = 0;
            image.setFillColor(sf::Color::Transparent);
            render = false;
            //count = GUI::Label();
        }

        void send_back()
        {
            if (item.item)
            {
                inventory[index].add_item(index, item.item, item.current_capacity);
            }
            clear();
        }

    };

    virtual void init() override;
    virtual void awake() override;

    virtual void update() override;
    /// <summary>
    /// Currently Close all not working
    /// </summary>
    void toggle_inventory(InventoryNS::WindowToggle window_to_toggle);
    void set_external(InventoryWindow* inv);
    void remove_external();

private:
    Inventory* general_inventory;
    Inventory* combat_inventory;
    Inventory* external_inventory;

    InventoryWindow* general_window;
    InventoryWindow* combat_window;
    InventoryWindow* external_window;

    Hand hand;

    void handle_left_click();
    void handle_right_click();
    void handle_shift_left_click();

    int mouse_in_slot(InventoryWindow*& win, Inventory*& inv);
    bool mouse_in_bounds(InventoryWindow*& win, Inventory*& inv);
    int mouse_in_slot(InventoryWindow*& win);


};

}