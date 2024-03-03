@tool
class_name NetworkedHotbar
extends Hotbar

func _change_selection(new_index : int):
	if not multiplayer.is_server():
		_change_selection_rpc.rpc_id(1, new_index)
	else:
		_change_selection_rpc(new_index)


@rpc("any_peer")
func _change_selection_rpc(new_index : int):
	if not multiplayer.is_server():
		return
	_change_selection_response_rpc.rpc(new_index)
	

@rpc("call_local")
func _change_selection_response_rpc(new_index : int):
	internal_change_selection(new_index)
