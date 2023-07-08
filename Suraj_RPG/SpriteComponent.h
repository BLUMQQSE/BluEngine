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
    virtual void awake() override;
    virtual void update() override;

    virtual void set_world_position(const Vector2f pos) override;

    // Inherited via IData
    virtual Json::Value serialize_json() override;
    virtual void unserialize_json(Json::Value obj) override;

    virtual void set_active(bool active) override;

    virtual std::vector<Editor::SerializedVar> get_editor_values() override;
    virtual void editor_update() override;

    sf::Sprite& get_sprite();
    sf::Texture& get_texture_sheet();
    virtual void add_to_buffer(sf::View* view) override;
    const std::string get_file_path() const;
    const sf::Vector2i get_size() const;

    void set_sprite(std::string file_path);
    void set_size(int size_x, int size_y);

private:
    sf::Sprite sprite;
    sf::Texture* texture_sheet;

    Vector2i size = Vector2i(32, 32);
    std::string file_path;

    bool sorting_group = false;
    float y_pos_offset = 0.f;

};

}