@icon("res://addons/inventory-system-demos/icons/dropped_item_3d.svg")
class_name DroppedItem3D
extends Node3D

const Interactor = preload("../interaction_system/inventory_interactor.gd")

@export var item_id : String
@export var amount : int = 1
@export var item_properties : Dictionary
@export var is_pickable := true
@export var actions : Array
@export var default_description : String = "Get Item"


func get_interaction_position(_interaction_point : Vector3) -> Vector3:
	return position


func get_interact_actions(_interactor : Interactor) -> Array:
	var text = str(amount) + " X "
	var item = _interactor.database.get_item(item_id)
	if item != null:
		text += item.name
	else:
		text += item_id
	actions[0].description = default_description.replace("Item", text)
	return actions


func interact(character : Node, _action_index : int = 0):
	character.character_inventory_system.pick_to_inventory(self)
