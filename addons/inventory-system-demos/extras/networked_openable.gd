class_name NetworkedOpenable
extends Openable


func _ready() -> void:
	if Engine.is_editor_hint():
		return
	multiplayer.peer_connected.connect(_on_connected.bind())
	opened.connect(_on_opened.bind())
	closed.connect(_on_closed.bind())
	

func _on_connected(id):
	if not multiplayer.is_server():
		return
	if is_open:
		_opened_rpc.rpc_id(id)

func _on_opened(character : Node):
	if not multiplayer.is_server():
		return
	_opened_rpc.rpc(character.get_path())


func _on_closed(character : Node):
	if not multiplayer.is_server():
		return
	_closed_rpc.rpc(character.get_path())


@rpc
func _opened_rpc(character_node_path : NodePath):
	if multiplayer.is_server():
		return
	is_open = true
	opened.emit(get_node(character_node_path))


@rpc
func _closed_rpc(character_node_path : NodePath):
	if multiplayer.is_server():
		return
	is_open = false
	closed.emit(get_node(character_node_path))
