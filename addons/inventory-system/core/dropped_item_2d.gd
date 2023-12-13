@icon("res://addons/inventory-system/icons/dropped_item_2d.svg")
extends Node2D
class_name DroppedItem2D

@export var item : InventoryItem
@export var is_pickable := true
@export var actions : Array[InteractAction]

func _ready():
	actions[0].description = actions[0].description.replace("Item", item.name)


func get_interaction_position(interaction_point : Vector2) -> Vector2:
	return position


func get_actions(_interactor : InventoryInteractor) -> Array[InteractAction]:
	return actions


func interact(interactor : InventoryInteractor, _action_index : int = 0):
	interactor.inventory_handler.pick_to_inventory(self)
