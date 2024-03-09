extends Node
class_name	SyncHotbar

@export var hotbar : Hotbar

func _ready() -> void:
	multiplayer.peer_connected.connect(_on_connected.bind())
	print("_on_connected register2")
	hotbar.on_change_selection.connect(on_change_selection.bind())

func _on_connected(peer_id : int):
	if not multiplayer.is_server():
		return
	print("_on_connected register")
	


func on_change_selection(selection_index : int):
	print("try send")
	if not multiplayer.is_server():
		return
	print("send")
	change_selection_rpc.rpc(selection_index)
	
@rpc
func change_selection_rpc(selection_index : int):
	print("response")
	hotbar.change_selection(selection_index)
