extends Workbench


func interact(character : Node, _action_index : int = 0):
	if multiplayer.is_server():
		interact_rpc(character.get_path(), _action_index)
	else:
		interact_rpc.rpc_id(1, character.get_path(), _action_index)

@rpc("any_peer")
func interact_rpc(character_path : NodePath, _action_index : int = 0):
	super.interact(get_node(character_path), _action_index)
