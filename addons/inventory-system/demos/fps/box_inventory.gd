extends Node3D
class_name BoxInventory


func get_inventory() -> Inventory:
	return $Inventory


func _on_inventory_closed():
	_on_close()


func _on_inventory_opened():
	_on_open()


#@rpc
#func open_rpc():
#	inventory
#
#
#@rpc("call_local")
#func close_rpc():
#	_on_close()


func _on_open():
	$box.visible = true
	$boxOpen.visible = false
	$Open.play()


func _on_close():
	$box.visible = false
	$boxOpen.visible = true
	$Close.play()
