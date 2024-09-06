extends Campfire
class_name NetworkedCampfire

func _ready():
	super._ready()
	changed_burning_state.connect(_changed_burning_state)
	multiplayer.peer_connected.connect(_on_connected)


func check():
	if not multiplayer.is_server():
		return
	super.check()
	

func _on_connected(_id : int):
	_changed_burning_state(is_burning)


func _changed_burning_state(is_burning_now : bool):
	if not multiplayer.is_server():
		return
	_changed_burning_state_rpc.rpc(is_burning_now)


@rpc
func _changed_burning_state_rpc(is_burning_now : bool):
	if multiplayer.is_server():
		return
	self.is_burning = is_burning_now


func interact(character : Node, _action_index : int = 0):
	if multiplayer.is_server():
		interact_rpc(character.get_path(), _action_index)
	else:
		interact_rpc.rpc_id(1, character.get_path(), _action_index)

@rpc("any_peer")
func interact_rpc(character_path : NodePath, _action_index : int = 0):
	super.interact(get_node(character_path), _action_index)
