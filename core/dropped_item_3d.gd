@icon("res://addons/inventory-system/icons/dropped_item_3d.svg")
extends Node3D
class_name DroppedItem3D

@export var item : SlotItem
@export var is_pickable := true
@export var actions : Array[InteractAction]
@export var default_description : String = "Get Item"


func get_interaction_position(interaction_point : Vector3) -> Vector3:
	return position


func get_actions(_interactor : InventoryInteractor) -> Array[InteractAction]:
	actions[0].description = default_description.replace("Item", item.definition.name)
	return actions


func interact(interactor : InventoryInteractor, _action_index : int = 0):
	interactor.inventory_handler.pick_to_inventory(self)
