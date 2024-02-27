#include "interactor.h"
#include <godot_cpp/classes/input.hpp>

void Interactor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_inventory_handler_path", "inventory_handler_path"), &Interactor::set_inventory_handler_path);
	ClassDB::bind_method(D_METHOD("get_inventory_handler_path"), &Interactor::get_inventory_handler_path);
	ClassDB::bind_method(D_METHOD("set_crafter_path", "crafter_path"), &Interactor::set_crafter_path);
	ClassDB::bind_method(D_METHOD("get_crafter_path"), &Interactor::get_crafter_path);
	ClassDB::bind_method(D_METHOD("set_hotbar_path", "hotbar_path"), &Interactor::set_hotbar_path);
	ClassDB::bind_method(D_METHOD("get_hotbar_path"), &Interactor::get_hotbar_path);
	ClassDB::bind_method(D_METHOD("set_raycast_path", "raycast_path"), &Interactor::set_raycast_path);
	ClassDB::bind_method(D_METHOD("get_raycast_path"), &Interactor::get_raycast_path);
	ClassDB::bind_method(D_METHOD("set_camera_path", "camera_path"), &Interactor::set_camera_path);
	ClassDB::bind_method(D_METHOD("get_camera_path"), &Interactor::get_camera_path);
	ClassDB::bind_method(D_METHOD("get_raycast"), &Interactor::get_raycast);
	ClassDB::bind_method(D_METHOD("try_interact"), &Interactor::try_interact);
	ClassDB::bind_method(D_METHOD("get_actions", "node"), &Interactor::get_actions);
	ClassDB::bind_method(D_METHOD("interact_object", "node", "actions"), &Interactor::interact_object);
	ClassDB::bind_method(D_METHOD("interact_hand_item", "hand_node", "hand_actions"), &Interactor::interact_hand_item);
	ClassDB::bind_method(D_METHOD("set_actual_hand_object", "actual_hand_object"), &Interactor::set_actual_hand_object);
	ClassDB::bind_method(D_METHOD("get_inventory_handler"), &Interactor::get_inventory_handler);
	ClassDB::bind_method(D_METHOD("get_crafter"), &Interactor::get_crafter);
	ClassDB::bind_method(D_METHOD("get_hotbar"), &Interactor::get_hotbar);
	ADD_SIGNAL(MethodInfo("preview_interacted", PropertyInfo(Variant::ARRAY, "actions", PROPERTY_HINT_ARRAY_TYPE, vformat("%s/%s:%s", Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "InteractAction"))));
	// ADD_SIGNAL(MethodInfo("interacted", PropertyInfo(Variant::OBJECT, "object")));
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "inventory_handler_path"), "set_inventory_handler_path", "get_inventory_handler_path");
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "crafter_path"), "set_crafter_path", "get_crafter_path");
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "hotbar_path"), "set_hotbar_path", "get_hotbar_path");
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "raycast_path", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "RayCast3D, RayCast2D"), "set_raycast_path", "get_raycast_path");
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "camera_path", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "Camera3D, Camera2D"), "set_camera_path", "get_camera_path");
}

void Interactor::set_inventory_handler_path(NodePath new_inventory_handler_path) {
	inventory_handler_path = new_inventory_handler_path;
}

NodePath Interactor::get_inventory_handler_path() const {
	return inventory_handler_path;
}

void Interactor::set_crafter_path(NodePath new_crafter_path) {
	crafter_path = new_crafter_path;
}

NodePath Interactor::get_crafter_path() const {
	return crafter_path;
}

void Interactor::set_hotbar_path(NodePath new_hotbar_path) {
	hotbar_path = new_hotbar_path;
}

NodePath Interactor::get_hotbar_path() const {
	return hotbar_path;
}

void Interactor::set_raycast_path(NodePath new_raycast_path) {
	raycast_path = new_raycast_path;
}

NodePath Interactor::get_raycast_path() const {
	return raycast_path;
}

void Interactor::set_camera_path(NodePath new_camera_path) {
	camera_path = new_camera_path;
}

NodePath Interactor::get_camera_path() const {
	return camera_path;
}

Node *Interactor::get_raycast() const {
	return get_node_or_null(raycast_path);
}

void Interactor::try_interact() {
	InventoryHandler *inventory_handler = get_inventory_handler();
	ERR_FAIL_NULL_MSG(inventory_handler, "'inventory_handler' is null in Interactor!");

	Crafter *crafter = get_crafter();
	ERR_FAIL_NULL_MSG(crafter, "'crafter' is null in Interactor!");

	if (inventory_handler->is_open_any_inventory() || crafter->is_open_any_station())
		return;
	Node *raycast = get_raycast();
	ERR_FAIL_NULL_MSG(raycast, "'raycast' is null in Interactor!");
	Object *object = raycast->call("get_collider");
	last_interact_object = object;
	Vector2 pos = Vector2(0, 0);
	Node *camera_node = get_node_or_null(camera_path);
	ERR_FAIL_NULL_MSG(camera_node, "'camera' is null in Interactor!");
	if (object != nullptr && object->has_method("get_interaction_position") && camera_node != nullptr) {
		pos = camera_node->call("unproject_position", object->call("get_interaction_position", (raycast->call("get_collision_point"))));
	}
	Node *node = Object::cast_to<Node>(object);
	// if (node == nullptr)
	// 	return;
	TypedArray<InteractAction> total_actions;
	TypedArray<InteractAction> object_actions;
	TypedArray<InteractAction> hand_actions = get_actions(actual_hand_object);

	if (raycast->call("is_colliding")) {
		object_actions = get_actions(node);
	}
	total_actions.append_array(object_actions);
	total_actions.append_array(hand_actions);
	emit_signal("preview_interacted", total_actions, pos);
	interact_object(node, object_actions);
	interact_hand_item(actual_hand_object, hand_actions);
}

TypedArray<InteractAction> Interactor::get_actions(Node *node) const {
	TypedArray<InteractAction> actions;

	InventoryHandler *inventory_handler = get_inventory_handler();
	ERR_FAIL_NULL_V_MSG(inventory_handler, actions, "'inventory_handler' is null in Interactor!");

	Crafter *crafter = get_crafter();
	ERR_FAIL_NULL_V_MSG(crafter, actions, "'crafter' is null in Interactor!");

	if (inventory_handler->is_open_any_inventory() || crafter->is_open_any_station())
		return actions;
	if (node != nullptr && node->has_method("get_interact_actions")) {
		Variant actions_var = node->call("get_interact_actions", this);
		TypedArray<InteractAction> actions = TypedArray<InteractAction>(actions_var);
	}
	return actions;
}

void Interactor::interact_object(Node *node, TypedArray<InteractAction> actions) {
	for (size_t i = 0; i < actions.size(); i++) {
		Ref<InteractAction> action = actions[i];
		if (Input::get_singleton()->is_action_just_pressed(action->get_input())) {
			node->call("interact", this, action->get_code());
			return;
		}
	}
}

void Interactor::interact_hand_item(Node *hand_node, TypedArray<InteractAction> hand_actions) {
	for (size_t i = 0; i < hand_actions.size(); i++) {
		Ref<InteractAction> action = hand_actions[i];
		if (Input::get_singleton()->is_action_just_pressed(action->get_input())) {
			hand_node->call("interact", this, action->get_code());
			return;
		}
	}
}

void Interactor::set_actual_hand_object(Node *new_actual_hand_object) {
	actual_hand_object = new_actual_hand_object;
}

InventoryHandler *Interactor::get_inventory_handler() const {
	Node *inventory_handler_node = get_node_or_null(inventory_handler_path);
	if (inventory_handler_node == nullptr)
		return nullptr;
	InventoryHandler *inventory_handler = Object::cast_to<InventoryHandler>(inventory_handler_node);
	return inventory_handler;
}

Crafter *Interactor::get_crafter() const {
	Node *crafter_node = get_node_or_null(crafter_path);
	if (crafter_node == nullptr)
		return nullptr;
	Crafter *crafter = Object::cast_to<Crafter>(crafter_node);
	return crafter;
}

Hotbar *Interactor::get_hotbar() const {
	Node *hotbar_path_node = get_node_or_null(hotbar_path);
	if (hotbar_path_node == nullptr)
		return nullptr;
	Hotbar *hotbar = Object::cast_to<Hotbar>(hotbar_path_node);
	return hotbar;
}
