@icon("res://addons/inventory-system/icons/dropped_item_3d.svg")
class_name DroppedItem3D
extends Node3D

@export var item : Item
@export var is_pickable := true
@export var actions : Array[InteractAction]
@export var default_description : String = "Get Item"


func get_interaction_position(interaction_point : Vector3) -> Vector3:
	return position


func get_interact_actions(_interactor : Interactor) -> Array[InteractAction]:
	actions[0].description = default_description.replace("Item", item.definition.name)
	return actions


func interact(character : Node, _action_index : int = 0):
	character.character_inventory_system.pick_to_inventory(self)
