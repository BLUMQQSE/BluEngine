#pragma once
namespace bm98
{
class PrefabEditor
{
public:
    PrefabEditor();
    virtual ~PrefabEditor();

    void update_sfml(sf::Event sfEvent);
    virtual void update();
    virtual void late_update();
    virtual void fixed_update();
    virtual void render(sf::View* = nullptr);

};
}
