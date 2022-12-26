#pragma once
#include "Component.h"
#include "IRenderable.h"
namespace bm98
{
class SpriteComponent :
    public Component, public IRenderable
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
    virtual void add_to_buffer(sf::View* view) override;
    const std::string get_file_path() const;
    const sf::Vector2i get_size() const;
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

};

}