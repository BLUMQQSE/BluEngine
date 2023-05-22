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

    inline std::weak_ptr<Interactor> get_current_interactor() { return current_interactor; }
    inline float get_duration() { return interaction_duration; }
    inline Interaction::Type get_type() { return interaction_type; }

    virtual void late_update() override;

    /// <summary>
    /// Check if interactor is able to initiate an interaction.
    /// </summary>
    bool check_can_initiate(std::shared_ptr<Interactor> interactor);
    virtual void initiate_interaction(std::shared_ptr<Interactor> interactor);
    
    virtual void exit_interaction();

protected:

    std::string interaction_prompt;
    std::string failed_interaction_prompt; 

    std::string debug_instant_message = " INITIATING INSTANT INTERACTION WITH ";
    std::string debug_initiate_message = " INITIATING INTERACTION WITH ";

    std::string debug_exit_message = " EXITING INTERACTION WITH ";
    
    bool busy = false;
    Interaction::Type interaction_type = Interaction::Type::INSTANT;
    bool require_player_input = false;

    Delay input_delay;
    
    float interaction_duration;
    sf::Clock interaction_timer;
    
    float timer_as_float;

    std::weak_ptr<Interactor> current_interactor = std::shared_ptr<Interactor>();
    Interaction::Priority interaction_priority;
    std::string priority_as_string;

    
    bool check_interaction_complete();

    virtual Json::Value serialize_json() override;
    virtual void unserialize_json(Json::Value obj) override;

    virtual std::vector<Editor::SerializedVar> get_editor_values() override;

    /// <summary>
    /// Handle interaction which only occurs once
    /// </summary>
    virtual void handle_instant_interaction();

    virtual void handle_interaction();


private:
};



}