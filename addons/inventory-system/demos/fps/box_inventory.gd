extends Node3D
class_name BoxInventory


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
