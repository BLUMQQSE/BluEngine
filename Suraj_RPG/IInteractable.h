#pragma once
#include "Component.h"
#include "Delay.h"
namespace bm98
{

class GameObject;
class Interactor;


class IInteractable : public Component
{

public:
    IInteractable() {}
    virtual ~IInteractable() {}
    
    inline std::string get_interaction_prompt() { return interaction_prompt; }
    inline std::string get_failed_interaction_promp() { return failed_interaction_prompt; }
    inline Interaction::Priority get_priority() { return interaction_priority; }

    inline const bool is_busy() const { return busy; }

    inline Interactor* get_current_interactor() { return current_interactor; }
    inline float get_duration() { return interaction_duration; }
    inline Interaction::Type get_type() { return interaction_type; }

    virtual void late_update() override;

    /// <summary>
    /// Check if interactor is able to initiate an interaction.
    /// </summary>
    bool check_can_initiate(Interactor* interactor);
    virtual void initiate_interaction(Interactor* interactor);
    
    virtual void exit_interaction();

protected:

    std::string interaction_prompt;
    std::string failed_interaction_prompt;
    
    bool busy = false;
    Interaction::Type interaction_type = Interaction::Type::INSTANT;
    bool require_player_input = false;

    Delay input_delay;
    
    float interaction_duration;
    sf::Clock interaction_timer;
    
    float timer_as_float;

    Interactor* current_interactor;
    Interaction::Priority interaction_priority;
    std::string priority_as_string;

    
    bool check_interaction_complete();

    virtual Json::Value serialize_json() override;
    virtual void unserialize_json(Json::Value obj) override;

    virtual std::vector<Editor::SerializedVar> get_editor_values();

    /// <summary>
    /// Handle interaction which only occurs once
    /// </summary>
    virtual void handle_instant_interaction();

    virtual void handle_interaction();


private:
};



}