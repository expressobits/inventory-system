@icon("res://addons/inventory-system/icons/dropped_item_3d.svg")
extends Node3D
class_name DroppedItem3D

@export var item : InventoryItem
@export var is_pickable := true


func get_interaction_position(interaction_point : Vector3) -> Vector3:
	return position


func get_interact_preview_message(_interactor : InventoryInteractor) -> String:
	return "[E] to Get "+item.name


func interact(interactor : InventoryInteractor):
	if Input.is_action_just_pressed("interact"):
		interactor.inventory_handler.pick_to_inventory(self)
