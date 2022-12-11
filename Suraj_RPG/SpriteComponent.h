#pragma once
#include "Component.h"
namespace bm98
{
class SpriteComponent :
    public Component
{
public:
    SpriteComponent();
    SpriteComponent(std::string file_path);
    virtual ~SpriteComponent();

    virtual void init() override;
    virtual void update() override;
    // Inherited via IData
    virtual Json::Value serialize_json() override;
    virtual void unserialize_json(Json::Value obj) override;

    sf::Sprite& get_sprite();
    sf::Texture& get_texture_sheet();
    SortingLayer get_layer();
    int get_order();

    void set_sprite(std::string file_path);
    void set_size(int size_x, int size_y);
    void set_position(float x, float y);
    void set_layer(SortingLayer sorting_layer);
    void set_z_order(int order);

private:
    sf::Sprite sprite;
    sf::Texture texture_sheet;

    sf::Vector2i size = sf::Vector2i(32, 32);
    std::string file_path;
    const std::string sprite_path = "Resources/Images/Sprites/";
    SortingLayer layer;
    int z_order;

};

}