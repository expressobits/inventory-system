extends Node3D
class_name BoxInventory


func get_inventory() -> Inventory:
	return $Inventory


func _on_inventory_closed():
	$box.visible = true
	$boxOpen.visible = false
	$Open.play()


func _on_inventory_opened():
	$box.visible = false
	$boxOpen.visible = true
	$Close.play()
