extends Inventory
class_name NetworkedInventory


func _ready():
	multiplayer.peer_connected.connect(_on_connected.bind())
	super._ready()


func _on_connected(id):
	if is_multiplayer_authority():
		opened.connect(_on_opened.bind())
		closed.connect(_on_closed.bind())


func _on_opened():
	opened_rpc.rpc()


func _on_closed():
	closed_rpc.rpc()


@rpc
func opened_rpc():
	if is_multiplayer_authority():
		return
	is_open = true
	emit_signal("opened")
	
@rpc
func closed_rpc():
	if is_multiplayer_authority():
		return
	is_open = false
	emit_signal("closed")
