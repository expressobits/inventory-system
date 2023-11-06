class_name PlaceableHandItem3D
extends HandItem3D


@export var drop_node_path : NodePath = "../../../.."
@onready var drop_node_parent : Node = get_node(drop_node_path)
@export var group_name_for_place_area : String = "placeable_area"
@export var property_from_item_for_object_scene : String = "placeable"
@export var input_for_place_item : String = "place_hand_item"
@export var preview_3d : PackedScene

var preview : Node3D

func _ready():
	preview = self.duplicate(0)
	preview.rotation = Vector3.ZERO
	add_child(preview)
	preview.position.x = 2


func get_interact_preview_message(interactor : InventoryInteractor) -> String:
	if  can_preview(interactor):
		return ""
	else:
		return "[Mouse Right] to place"


func _process(delta):
	if interactor != null:
		if can_preview(interactor):
			preview.global_position = interactor.raycast.get_collision_point()
			preview.visible = true
		else:
			preview.visible = true


func can_preview(interactor : InventoryInteractor) -> bool:
	if not visible:
		return false
	var object = interactor.last_interact_object
	var node : Node3D = object as Node3D
	if node == null:
		return false
	if not node.is_in_group(group_name_for_place_area):
		return false
	var item = interactor.hotbar.get_selected_item()
	if item == null:
		return false
	if not item.properties.has(property_from_item_for_object_scene):
		return false
	return true


func interact(interactor : InventoryInteractor):
	var object = interactor.last_interact_object
	var node : Node3D = object as Node3D
	if node != null:
		if node.is_in_group(group_name_for_place_area):
			if Input.is_action_just_pressed(input_for_place_item):
				var item = interactor.hotbar.get_selected_item()
				if item != null:
					_place_item(interactor, item, interactor.raycast.get_collision_point())


func _place_item(interactor : InventoryInteractor, item : InventoryItem, position_to_place : Vector3):
	# TODO Add 3D Preview
	if !item.properties.has(property_from_item_for_object_scene):
		return
	var path = item.properties[property_from_item_for_object_scene]
	var res = load(path)
	if res is PackedScene:
		var scene = res as PackedScene
		var obj = scene.instantiate()
		obj.position = position_to_place 
		drop_node_parent.add_child(obj)
		interactor.inventory_handler.inventory.remove(item)
