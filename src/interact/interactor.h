#ifndef INTERACTOR_CLASS_H
#define INTERACTOR_CLASS_H

#include "base/node_inventories.h"
#include "core/hotbar.h"
#include "core/inventory_handler.h"
#include "craft/crafter.h"
#include "interact_action.h"
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/ray_cast3d.hpp>


using namespace godot;

class Interactor : public NodeInventories {
	GDCLASS(Interactor, NodeInventories);

private:
	NodePath inventory_handler_path = NodePath("../InventoryHandler");
	NodePath crafter_path = NodePath("../Crafter");
	NodePath hotbar_path = NodePath("../Hotbar");
	NodePath raycast_path;
	NodePath camera_path;
	Object *last_interact_object = nullptr;
	Node *actual_hand_object = nullptr;

protected:
	static void _bind_methods();

public:
	void set_inventory_handler_path(NodePath new_inventory_handler_path);
	NodePath get_inventory_handler_path() const;
	void set_crafter_path(NodePath new_crafter_path);
	NodePath get_crafter_path() const;
	void set_hotbar_path(NodePath new_hotbar_path);
	NodePath get_hotbar_path() const;
	void set_raycast_path(NodePath new_raycast_path);
	NodePath get_raycast_path() const;
	void set_camera_path(NodePath new_camera_path);
	NodePath get_camera_path() const;
	Node *get_raycast() const;
	// 🫴 Interact System
	void try_interact();
	TypedArray<InteractAction> get_actions(Node *node) const;
	void interact_object(Node *node, TypedArray<InteractAction> actions);
	void interact_hand_item(Node *hand_node, TypedArray<InteractAction> hand_actions);
	void set_actual_hand_object(Node *new_actual_hand_object);
	InventoryHandler* get_inventory_handler() const;
	Crafter* get_crafter() const;
	Hotbar* get_hotbar() const;
};

#endif //INTERACTOR_CLASS_H