extends Node3D
class_name BoxInventory

@onready var inventory : Inventory = $Inventory


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


func get_interact_preview_message(_interactor : InventoryInteractor) -> String:
	if inventory.is_open:
		return ""
	return "[E] to Open Inventory"


func interact(interactor : InventoryInteractor):
	if Input.is_action_just_pressed("interact"):
		interactor.get_parent().open_inventory(inventory)
