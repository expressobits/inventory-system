@tool
extends Hotbar
class_name NetworkedHotbar


func set_selection_index(new_index : int):
	if not multiplayer.is_server():
		set_selection_index_rpc.rpc_id(1, new_index)
	else:
		set_selection_index_rpc(new_index)


@rpc("any_peer","call_local")
func set_selection_index_rpc(new_index : int):
	if not multiplayer.is_server():
		return
	super.set_selection_index(new_index)
	set_selection_index_response_rpc.rpc(new_index)
	

@rpc
func set_selection_index_response_rpc(new_index : int):
	super.set_selection_index(new_index)
