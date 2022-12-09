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

private:
    sf::Sprite sprite;
    sf::Texture texture_sheet;
};

}