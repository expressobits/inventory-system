@tool
class_name InventoryInteractor
extends NodeInventorySystemBase

signal preview_interacted(actions : Array[InteractAction], position_screen : Vector2)
signal clear_preview
signal interacted(object : Node)

@export_node_path("InventoryHandler") var inventory_handler_path := NodePath("../InventoryHandler")
@onready var inventory_handler : InventoryHandler = get_node(inventory_handler_path)
@export_node_path("Hotbar") var hotbar_path := NodePath("../Hotbar")
@onready var hotbar : Hotbar = get_node(hotbar_path)
@export var raycast : RayCast3D
@export var camera_3d : Camera3D

var last_interact_object : Node
var actual_hand_object : Node


## 🫴 Interact System
func try_interact():
	var object = raycast.get_collider()
	last_interact_object = object
	var pos : Vector2 = Vector2.ZERO
	if object != null and object.has_method("get_interaction_position") and camera_3d != null:
		pos = camera_3d.unproject_position(object.get_interaction_position(raycast.get_collision_point()))
	
	if not raycast.is_colliding():
		clear_preview.emit()
		return
	
	var node = object as Node
	if node == null or not node.has_method("get_actions"):
		clear_preview.emit()
		return
	
	var actions = get_actions(node)
	preview_interacted.emit(actions, pos)
	
	interact_object(object, actions)
#	interact_hand_item(actual_hand_object)


func get_actions(node : Node) -> Array[InteractAction]:
	var item = hotbar.get_selected_item()
	var actions = node.get_actions(self)
	return actions


func interact_object(object : Node, actions : Array[InteractAction]):
	for action in actions:
		if Input.is_action_just_pressed(action.input):
			object.interact(self, action.code)
			return


func interact_hand_item(hand_object):
	if actual_hand_object == null:
		return
	if not actual_hand_object.has_method("interact"):
		return
	actual_hand_object.interact(self)	
	return
