extends Shelf
class_name NetworkedShelf


func interact(character : Node, action_index : int = 0):
	if not multiplayer.is_server():
		interact_rpc.rpc_id(1, character.get_path(), action_index, slot_index)
	else:
		interact_rpc(character.get_path(), action_index, slot_index)

@rpc("any_peer")
func interact_rpc(character_node_path : NodePath, action_index : int, slot_index):
	if not multiplayer.is_server():
		return
	var node = get_node(character_node_path)
	if node == null:
		return
	super.interact_with_slot(node, action_index, slot_index)
