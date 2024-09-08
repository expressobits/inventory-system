@icon("res://addons/inventory-system-demos/icons/dropped_item_2d.svg")
class_name DroppedItem2D
extends Node2D

const Interactor = preload("../interaction_system/inventory_interactor.gd")

@export var item : ItemDefinition
@export var is_pickable := true
@export var actions : Array

func _ready():
	actions[0].description = actions[0].description.replace("Item", item.name)


func get_interaction_position(interaction_point : Vector2) -> Vector2:
	return position


func get_interact_actions(_interactor : Interactor) -> Array:
	return actions


func interact(character : Node, _action_index : int = 0):
	character.character_inventory_system.pick_to_inventory(self)
