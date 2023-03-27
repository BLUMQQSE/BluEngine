#pragma once
#include "globals.h"
#include "Component.h"
#include "Collisions.h"
namespace bm98
{

class Collider;
class IInteractable;

namespace core
{
class LayerMask;
}
class Interactor : public Component
{

public:
	inline const bool is_interacting() const { return interacting; }

	/// <summary>
	/// This method relies on input, and therefor should only be called from an
	/// update method.
	/// </summary>
	void interact();
	/// <summary>
	/// This method relies on input, and therefor should only be called from an
	/// update method.
	/// </summary>
	/// <param name="interactable"></param>
	void interact(IInteractable* interactable);
	void cancel_interaction();

	virtual void fixed_update() override;

	/// <summary>
	/// Should only be called by IInteractable on exit_interaction().
	/// </summary>
	void remove_interactable();

protected:
	// number of interactables near
	int count = 0;
	float interaction_radius = 0;
	PhysicsNS::LayerMask interactable_mask;
	
	bool interacting;

	IInteractable* potential_interactable = nullptr;
	IInteractable* current_interactable = nullptr;

	virtual Json::Value serialize_json();
	virtual void unserialize_json(Json::Value obj);
	
	virtual std::vector<Editor::SerializedVar> get_editor_values();

};

}
