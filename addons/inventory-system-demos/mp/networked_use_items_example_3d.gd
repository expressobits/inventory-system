class_name NetworkedUseItemsExample3D
extends UseItemsExample3D


func use_item_selected_in_hotbar():
	if not multiplayer.is_server():
		use_item_selected_in_hotbar_rpc.rpc_id(1)
	else:
		use_item_selected_in_hotbar_rpc()


@rpc
func use_item_selected_in_hotbar_rpc():
	if not multiplayer.is_server():
		return
	super.use_item_selected_in_hotbar()
