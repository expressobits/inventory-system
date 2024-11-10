@icon("res://addons/inventory-system-demos/icons/dropped_item_2d.svg")
class_name DroppedItem2D
extends Node2D

const Interactor = preload("../interaction_system/inventory_interactor.gd")

@export var item_id : String
@export var amount : int = 1
@export var item_properties : Dictionary
@export var is_pickable := true
@export var actions : Array
@export var default_description : String = "Get Item"


func get_interaction_position(interaction_point : Vector2) -> Vector2:
	return position


func get_interact_actions(_interactor : Interactor) -> Array:
	var text = ""
	if amount > 1:
		text += str(amount) + " X "
	actions[0].description = default_description.replace("Item", text + item_id)
	return actions


func interact(character : Node, _action_index : int = 0):
	character.character_inventory_system.pick_to_inventory(self)
