class_name PlaceableHandItem3D
extends HandItem3D

const Interactor = preload("../interaction_system/inventory_interactor.gd")

@export var group_name_for_place_area : String = "placeable_area"
@export var property_from_item_for_object_scene : String = "placeable"
@export var input_for_place_item : String = "place_hand_item"
@export var preview_3d : PackedScene

@export var place_action : InteractAction
var interactor : Interactor

var preview : Node3D

func _ready():
	preview = self.duplicate(0)
	preview.rotation = Vector3.ZERO
	add_child(preview)
	preview.position.x = 2


func _process(delta):
	if interactor != null:
		if can_preview(interactor):
			preview.global_position = interactor.raycast.get_collision_point()
			preview.visible = true
		else:
			preview.visible = false


func get_interact_actions(_interactor : Interactor) -> Array:
	self.interactor = _interactor
	if can_preview(_interactor):
		return [place_action]
	return []


func can_preview(interactor : Interactor) -> bool:
	if not visible:
		return false
	var object = interactor.last_interact_object
	if object == null:
		return false
	var node : Node3D = object as Node3D
	if not node.is_in_group(group_name_for_place_area):
		return false
	
	var stack = interactor.get_parent().hotbar.get_stack_on_selection()
	if stack == null:
		return false
	var definition = interactor.database.get_item(stack.item_id)
	if definition == null:
		return false
	if not definition.properties.has(property_from_item_for_object_scene):
		return false
	return true


func interact(character : Node, _action_code : int = 0):
	var object = interactor.last_interact_object
	var node : Node3D = object as Node3D
	if node != null:
		if node.is_in_group(group_name_for_place_area):
			var stack = interactor.get_parent().hotbar.get_stack_on_selection()
			if stack != null:
				## TODO Thinking best catch interactor responses
				character.character_inventory_system.interactor.get_node("ObjectPlacer").place_item(stack.item_id, interactor.raycast.get_collision_point(), rotation)
