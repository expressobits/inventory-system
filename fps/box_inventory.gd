extends Node3D
class_name BoxInventory

@onready var inventory : Inventory = $Inventory
@export var actions : Array[InteractAction]

func get_inventory() -> Inventory:
	return $Inventory


func _on_inventory_closed():
	_on_close()


func _on_inventory_opened():
	_on_open()


func _on_open():
	$box.visible = false
	$boxOpen.visible = true
	$Open.play()


func _on_close():
	$box.visible = true
	$boxOpen.visible = false
	$Close.play()


func get_interaction_position(_interaction_point : Vector3) -> Vector3:
	return position


func get_actions(_interactor : InventoryInteractor) -> Array[InteractAction]:
	if inventory.is_open:
		return []
	return actions


func interact(interactor : InventoryInteractor, _action_index : int = 0):
	interactor.get_parent().open_inventory(inventory)
