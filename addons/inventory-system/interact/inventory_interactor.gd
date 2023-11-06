@tool
class_name InventoryInteractor
extends NodeInventorySystemBase

signal preview_interacted(object : Node, hand_object : Node, position_screen : Vector2)
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
	preview_interacted.emit(object, actual_hand_object, pos)
	
	if not raycast.is_colliding():
		return
	interact_object(object)
	interact_hand_item(actual_hand_object)


func interact_object(object : Node):
	var node = object as Node
	if node == null or not node.has_method("interact"):
		return
	var item = hotbar.get_selected_item()
	object.interact(self)
	return


func interact_hand_item(hand_object):
	if actual_hand_object == null:
		return
	if not actual_hand_object.has_method("interact"):
		return
	actual_hand_object.interact(self)	
	return
