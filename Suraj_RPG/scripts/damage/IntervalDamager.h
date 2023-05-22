#include "Damager.h"


namespace bm98
{


class IntervalDamager :
    public Damager
{
public:
    IntervalDamager();
    virtual ~IntervalDamager();

    virtual void update() override;
    virtual void fixed_update() override;

    virtual std::vector<Editor::SerializedVar> get_editor_values() override;

protected:
    virtual Json::Value serialize_json() override;
    virtual void unserialize_json(Json::Value obj) override;


    float hit_interval = 1;
    std::vector<std::pair<int, Timer>> hit_delay;

};

}